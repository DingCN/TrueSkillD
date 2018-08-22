#include "stdafx.h"
#include "PriorFactor.h"
#include "Gaussian.h"
#include <cmath>

PriorFactor::PriorFactor(std::vector<Variable*> pVars, const RatingValue & val, const double & dynamic = 0) : Factor(pVars)
{
	this->val = val;
	this->dynamic = dynamic;
}
PriorFactor::PriorFactor()
{
}
PriorFactor::~PriorFactor()
{
}

double PriorFactor::down()
{
	double sigma = sqrt(this->val.sigma * this->val.sigma + this->dynamic * this->dynamic);
	double pi = 1 / (sigma*sigma);
	double tau = pi * this->val.mu;
	return this->pVars[0]->update_value(this, pi, tau);
}

double PriorFactor::up() {
	return 0.0;
}
