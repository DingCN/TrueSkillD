#ifndef  PRIORFACTOR_INC
#define  PRIORFACTOR_INC

#include "Factor.h"

class PriorFactor: public Factor
{
public:
	PriorFactor(std::vector<Variable*> pVar, const RatingValue & val, const double & dynamic);
	PriorFactor();
	~PriorFactor();
	double down();
	double up();
private:
	double dynamic;
	RatingValue val;
};

#endif