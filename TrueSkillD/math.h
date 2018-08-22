#ifndef  MATH_INC
#define  MATH_INC

#include <cmath>


class Math
{
public:
	Math();
	double math_erfc(double x);
	double math_pdf(double x, double mu = 0, double sigma = 1);
	double math_cdf(double x, double mu = 0, double sigma = 1);
	double math_erfcinv(double y);
	double math_ppf(double x, double mu = 0, double sigma = 1);
	~Math();
};






#endif   /* ----- #ifndef MATH_INC  ----- */
/* vi:set ts=4: */