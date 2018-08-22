#include "stdafx.h"
#include "Variable.h"
#include "Factor.h"
#include <cmath>
#include <algorithm>
#include "TrueSkillD.h"
#include "iostream"
Variable::Variable(double pi, double tau) : Gaussian(pi, tau)
{
}

Variable::~Variable()
{
}

double Variable::set(RatingValue & Value)
{
	double delta = this->delta(Value);
	this->pi = Value.pi;
	this->tau = Value.tau;
	this->mu = Value.mu;
	this->sigma = Value.sigma;
	return delta;
}

double Variable::delta(RatingValue & Value)
{
	double pi_delta = abs(this->pi - Value.pi);
	if (pi_delta == INFINITY) {
		return 0;
	}
	else {
		return std::max(abs(this->tau - Value.tau), sqrt(pi_delta));
	}
}

double Variable::update_message(Factor * pFactor, double pi, const double tau) // message = None) :
{
	RatingValue message = GetVariableValues(Variable(pi, tau));////////////////////////////////////////
	RatingValue old_message;
	RatingValue value;
	if (m_Message.find(pFactor) != m_Message.end()) {
		old_message = this->m_Message.find(pFactor)->second;
		m_Message.find(pFactor)->second = GetVariableValues(Variable(message.pi, message.tau));
		value = GetVariableValues(Variable(this->get_pi() - old_message.pi + message.pi, this->get_tau() - old_message.tau + message.tau));
	}
	else {
		std::cout << "ERROR, MESSAGE NOT FOUND IN MAP in Variable::update_message\n";
	}
	return this->set(value);
}

double Variable::update_value(Factor * pFactor, const double pi, const double tau)//, value = None)
{
	RatingValue value = GetVariableValues(Variable(pi, tau));
	if (this->m_Message.find(pFactor) != this->m_Message.end()) {//already has a previous message
		RatingValue old_message = m_Message.find(pFactor)->second;
		m_Message.find(pFactor)->second = GetVariableValues(Variable(value.pi + old_message.pi - this->get_pi(), value.tau + old_message.tau - this->get_tau()));
	}
	else {
		std::cout << "ERROR, MESSAGE NOT FOUND IN MAP in Variable::update_value \n";
	}
	return this->set(value);
}