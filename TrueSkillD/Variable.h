#ifndef  VARIABLE_INC
#define  VARIABLE_INC

#include "Gaussian.h"
#include "Factor.h"
#include <map>
#include "TrueSkillD.h"

class Factor;

class Variable : public Gaussian
{
public:
	Variable(double pi = 0, double tau = 0);
	~Variable();

	double set(RatingValue & Value);

	double delta(RatingValue & Value);
	double update_message(Factor * pFactor, double pi = 0, const double tau = 0);
	double update_value(Factor * pFactor, const double pi = 0, const double tau = 0);
	std::map<Factor*, RatingValue> m_Message;
private:

};

#endif
