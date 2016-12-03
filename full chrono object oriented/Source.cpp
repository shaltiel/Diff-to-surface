//chrono with simec
#include <iostream>
#include <math.h>
#include <time.h> 
#include "classes\simec.h"
typedef double real;

using std::vector;
int main(int argc, const char **argv){


	//intervals & cells sizes-----------------------------------
	real T = 0, dt = 1e-8, Da = 1;
	real dR = 0.125, gammaR = 1.0, dZ = 0.125, gammaZ = 1.0, gammaT = 1.01;
	real Rmax = 2, Zmax = 2, Tmax = 1000;
	//----------------------------------------------------------

	//------------MESH------------------------------------------
	//Z vector
	Grid Zgrid(dZ, gammaZ);
	Zgrid.open_node(Zmax);
	int Zn = Zgrid.get_node(1) - 1;
	//R vector
	Grid Rgrid(dR, gammaR);
	Rgrid.new_node(1);
	Rgrid.open_node(Rmax);
	int Rd = Rgrid.get_node(1) - 1, Rn = Rgrid.get_node(2) - 1;
	//---------------------------------------------------------

	specie A(Rgrid, Zgrid, 1.0);
	PreTomas phys(Rgrid, Zgrid);

	//-----------World-----------------------------------------
	int Domain = A.world.make_domain(0, 0, Rn, Zn);
	int Electrode = A.world.make_edge(0, 0, Rd, 0);
	int BulkZn = A.world.make_edge(0, Zn, Rn - 1, Zn);
	int BulkRn = A.world.make_edge(Rn, 0, Rn, Zn);
	int Symmetry = A.world.make_edge(0, 0, 0, Zn);
	int Sheet = A.world.make_edge(Rd + 1, 0, Rn, 0);
	//---------------------------------------------------------	

	clock_t begin = clock();

	//---------------Simulation--------------------------------
	while (T < Tmax)
	{
		//------------------define Z elements-----------------
		for (int n = 0; n < Rn; n++)
		{
			for (int m = 0; m <= Zn; m++)
			{	    //domain equations Z directon
				if (A.world.is(n, m, Domain) || A.world.is(n, m, Symmetry) || A.world.is(n, m, BulkZn))
				{
					if (T == 0)	*A.set_az(n, m) = phys.Z_a(n, m);
					*A.set_bz(n, m) = phys.Z_b(n, m, dt, Da);
					if (T == 0) *A.set_gz(n, m) = phys.Z_g(n, m);
					*A.set_dz(n, m) = phys.Z_d(&A, n, m, dt, Da);
				}
				//boundary conditions Z directon
				if (T == 0)
				{
					if (A.world.is(n, m, Electrode)) phys.nernst(&A, 'Z', n, m, 20);
					if (A.world.is(n, m, Sheet)) phys.insulation(&A, 'Z', n, m);
					if (A.world.is(n, m, BulkZn)) phys.bulk(&A, 'Z', n, m);
				}

			}
			
		}
		//----------solve Z elements------------------------
		for (int n = 0; n < Rn; n++)
		{
			tomas * tomA = new tomas(&A, 'Z', n);
			(*tomA).modify_g(); (*tomA).modify_d(); (*tomA).solve_c();
			delete tomA;
		}

		//----------Current  display---------------------------
		std::cout << '\n' << T << '\t' << A.current();

		//------------------define R elements-----------------
		for (int n = 0; n <= Rn; n++)
		{
			for (int m = 1; m < Zn; m++)
			{
				if (A.world.is(n, m, Domain))
				{
					if (T == 0) *A.set_ar(n, m) = phys.R_a(n, m);
					*A.set_br(n, m) = phys.R_b(n, m, dt, Da);
					if (T == 0) *A.set_gr(n, m) = phys.R_g(n, m);
					*A.set_dr(n, m) = phys.R_d(&A, n, m, dt, Da);
					
				}
				if (T == 0)
				{
					if (A.world.is(n, m, BulkRn)) phys.bulk(&A, 'R', n, m);
					if (A.world.is(n, m, Symmetry)) phys.insulation(&A, 'R', n, m);
				}
			}
		}
		//----------solve R elements------------------------
		for (int m = 1; m < Zn; m++)
		{
			tomas * tomA = new tomas(&A, 'R', m);
			(*tomA).modify_g(); (*tomA).modify_d(); (*tomA).solve_c();
			delete tomA;
		}
		//increase time step
		T = T + dt;
		dt = dt*gammaT;
	}
	//---------------------------------------------------------------------------	
	clock_t end = clock();
	cout << "\n" << "CPU chrono:" << (end - begin) << "(ms)" << "\n";
	return 0;
}