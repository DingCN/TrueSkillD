#ifndef  FACTOR_INC
#define  FACTOR_INC
#include "Variable.h"
#include <vector>

class Variable;

class Factor
{
public:
	Factor(std::vector<Variable*> pVars);
	Factor();
	~Factor();
	int ConstructMessageMap(std::vector<Variable*> pVars);
	virtual double down();
	virtual double up();
	//int print_vars();
	std::vector<Variable*> pVars;
protected:

};

#endif