#include "stdafx.h"
#include "Variable.h"
#include "LikelihoodFactor.h"
#include <iostream>
#include "TrueSkillD.h"


LikelihoodFactor::LikelihoodFactor(Variable* pMean_var, Variable* pValue_var, double variance, std::vector<Variable*> pVars) :Factor(pVars)
{
	this->pMean = pMean_var;
	this->pValue = pValue_var;
	this->variance = variance;
}
LikelihoodFactor::LikelihoodFactor()
{
}
LikelihoodFactor::~LikelihoodFactor()
{
}

double LikelihoodFactor::calc_a(const RatingValue val)
{
	double ret = 1 / (1 + this->variance * val.pi);
	return ret;
}

double LikelihoodFactor::down()
{
	//P24
	RatingValue msg;
	if (pMean->m_Message.find(this) != pMean->m_Message.end()) {
		RatingValue denom = this->pMean->m_Message.find(this)->second;
		msg = GetVariableValues(Variable(this->pMean->get_pi() - denom.pi, this->pMean->get_tau() - denom.tau));
	}
	else {
		msg = GetVariableValues(Variable(this->pMean->get_pi(), this->pMean->get_tau()));
	}
	double a = this->calc_a(msg);
	return this->pValue->update_message(this, a*msg.pi, a* msg.tau);

}

double LikelihoodFactor::up()
{
	RatingValue msg;
	if (pValue->m_Message.find(this) != pValue->m_Message.end()) {
		RatingValue denom = this->pValue->m_Message.find(this)->second;
		msg = GetVariableValues(Variable(this->pValue->get_pi() - denom.pi, this->pValue->get_tau() - denom.tau));
	}
	else {
		msg = GetVariableValues(Variable(this->pValue->get_pi(), this->pValue->get_tau()));
	}
	double a = this->calc_a(msg);
	return this->pMean->update_message(this, a*msg.pi, a* msg.tau);
}
