#ifndef  SUMFACTOR_INC
#define  SUMFACTOR_INC

#include "Factor.h"
#include <vector>

class SumFactor : public Factor
{
public:
	SumFactor(Variable* pSum_var, std::vector<Variable*> pTerm_vars, std::vector<Variable*> pVars, std::vector<double> coeffs); //vars has all sum_vars and term_vars
	SumFactor();
	~SumFactor();

	double down();
	double up(int index = 0);
	double update(Variable* pVar, std::vector<Variable*> pVals, std::vector<RatingValue> msgs, std::vector<double> coeffs); //  coeffs is aj in the Math paper


private:
	Variable * pSum;
	std::vector<Variable*> pTerms;
	std::vector<double> coeffs;
};

#endif