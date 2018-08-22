#include "stdafx.h"
#include "SumFactor.h"
#include <iostream>
#include "TrueSkillD.h"
SumFactor::SumFactor(Variable* pSum_var, std::vector<Variable*> pTerm_vars, std::vector<Variable*> pVars, std::vector<double> coeffs) : Factor(pVars) //vars has all sum_vars and term_vars
{
	this->pSum = pSum_var;
	this->pTerms = pTerm_vars;
	this->coeffs = coeffs;
}
SumFactor::SumFactor()
{
}
SumFactor::~SumFactor()
{
}

double SumFactor::down()
{
	std::vector<Variable*> vals = this->pTerms;
	std::vector<RatingValue> msgs;
	for (size_t i = 0; i < vals.size(); i++) {
		if (vals[i]->m_Message.find(this) != vals[i]->m_Message.end()) {
			msgs.push_back(vals[i]->m_Message.find(this)->second);
		}
	}
	return this->update(this->pSum, vals, msgs, this->coeffs);
}

double SumFactor::up(int index)
{
	double coeff = this->coeffs[index];
	std::vector<double> coeffs;
	for (size_t i = 0; i < this->coeffs.size(); i++) {
		if (coeff == 0) {
			std::cout << "ZeroDivisionError";
			coeffs.push_back(0.0);
			continue;
		}
		if (i == index) {
			coeffs.push_back(1 / coeff);
		}
		else {
			coeffs.push_back(-(this->coeffs[i]) / coeff);
		}
	}
	std::vector<Variable*> vals = this->pTerms;
	vals[index] = this->pSum;
	std::vector<RatingValue> msgs;
	for (size_t i = 0; i < vals.size(); i++) {
		if (vals[i]->m_Message.find(this) != vals[i]->m_Message.end()) {
			msgs.push_back(vals[i]->m_Message.find(this)->second);
		}
	}
	return this->update(this->pTerms[index], vals, msgs, coeffs);
}

double SumFactor::update(Variable* pVar, std::vector<Variable*> pVals, std::vector<RatingValue> msgs, std::vector<double> coeffs) //  coeffs is aj in the Math paper
{
	double pi_inv = 0;
	double mu = 0;
	for (size_t i = 0; i < pVals.size(); i++) {//pVals and msgs should have the same size
		if (i >= msgs.size()) {
			std::cout << "ERROR, pVals and msgs should have the same size";
			break;
		}
		RatingValue div = GetVariableValues(Variable(pVals[i]->get_pi() - msgs[i].pi, pVals[i]->get_tau() - msgs[i].tau));
		mu += coeffs[i] * div.mu;
		if (pi_inv == INFINITY) {
			continue;
		}
		if (div.pi == 0) {
			std::cout << "ZERODIVISION";
			pi_inv = INFINITY;
		}
		pi_inv += coeffs[i] * coeffs[i] / div.pi;

	}
	double pi = 1 / pi_inv;
	if (pi == INFINITY) {
		std::cout << "ZERODIVISION";
	}
	double tau = pi * mu;
	return pVar->update_message(this, pi, tau);
}
