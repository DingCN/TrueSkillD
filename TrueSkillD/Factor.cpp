#include "stdafx.h"
#include "Factor.h"
#include <vector>
#include "TrueSkillD.h"
#include "Variable.h"
Factor::Factor(std::vector<Variable*> pVars)
{
	this->pVars = pVars;
}
Factor::Factor()
{
}

Factor::~Factor()
{
}

double Factor::down() {
	return 0.0;
}
double Factor::up() {
	return 0.0;
}

int Factor::ConstructMessageMap(std::vector<Variable*> pVars)
{
	for (size_t i = 0; i < pVars.size(); i++) {
		if (pVars[i]->m_Message.find(this) == pVars[i]->m_Message.end()) {
			RatingValue tmp;
			tmp.pi = 0;
			tmp.tau = 0;
			tmp.mu = 0;
			tmp.sigma = INFINITY;
			pVars[i]->m_Message.insert(std::pair<Factor*, RatingValue>(this, tmp)); // pi and tau should be set to 0
		}
	}
	return 0;
}