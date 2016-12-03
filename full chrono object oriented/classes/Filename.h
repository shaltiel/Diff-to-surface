//menual: use addSRFilename filen;
//filen.set_name(Name3);
//Name3 = filen.addSR(1).txt();


#include <iostream>
#include <string>
#include <sstream> 
#include <iomanip>
class Filename {
	std::string name1;
public:
	void set_name(std::string);
	
	Filename& addSR(double sSR);
	

	std::string txt(){
		name1 += {'.', 't', 'x', 't', '\0'};
		return name1;
	}


};


//function implemetation 
void Filename::set_name(std::string name){
	name1 += name;
}


Filename& Filename::addSR(double sSR){
	std::ostringstream s;
	s << sSR;
	std::string numStr(s.str());
	if (s.str() == "0.000001"){ name1 += '1', 'u' ; }
	if (s.str() == "0.00001"){ name1 +=  '1', '0', 'u' ; }
	if (s.str() == "0.0001"){ name1 +=  '1', '0', '0', 'u' ; }
	if (s.str() == "0.001"){ name1 += '1', 'm'; }
	if (s.str() == "0.01"){ name1 +=  '1', '0', 'm' ; }
	if (s.str() == "0.1"){ name1 +=  '1', '0', '0', 'm' ; }
	if (s.str() == "1"){ name1 += '1' ; }
	if (s.str() == "10"){ name1 += '1', '0' ; }
	if (s.str() == "100"){ name1 +=  '1', '0', '0' ; }
	if (s.str() == "1000"){ name1 +=  '1', 'k' ; }
	if (s.str() == "10000"){ name1 +=  '1', '0', 'k' ; }
	if (s.str() == "100000"){ name1 +=  '1', '0', '0', 'k'; }
	if (s.str() == "1000000"){ name1 +=  '1', 'M' ; }
	if (s.str() == "10000000"){ name1 +=  '1', '0', 'M' ; }
	if (s.str() == "100000000"){ name1 +=  '1', '0', '0', 'M' ; }
	return *this;
	}