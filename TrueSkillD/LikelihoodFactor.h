#ifndef  LIKELIHOODFACTOR_INC
#define  LIKELIHOODFACTOR_INC
#pragma once
#include "Factor.h"


class LikelihoodFactor : public Factor
{
public:
	LikelihoodFactor(Variable* pMean_var, Variable* pValue_var, const double variance, std::vector<Variable *> pVars);
	LikelihoodFactor();
	~LikelihoodFactor();
	double calc_a(const RatingValue val);
	double down();
	double up();
private:
	Variable* pMean;
	Variable* pValue;
	double variance;
};
#endif
