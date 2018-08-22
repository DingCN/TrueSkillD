#include "stdafx.h"
#include<cmath>
#include "Math.h"



Math::Math()
{
}

Math::~Math()
{
}

double Math::math_erfc(double x)
{
	return erfc(x);
}
// Returns the probability of x, given the distribution described by mu and sigma.
double Math::math_pdf(double x, double mu, double sigma)
{
	//Constants
	static const double pi = 3.14159265359;
	return exp(-1 * (x - mu) * (x - mu) / (2 * sigma * sigma)) / (sigma * sqrt(2 * pi));
}

// Returns the probability of [-inf,x] of a Gaussian distribution
double Math::math_cdf(double x, double mu, double sigma)
{
	return 0.5 * (1 + erf((x - mu) / (sigma * sqrt(2.))));
}



double Math::math_erfcinv(double y)
{
	//The inverse function of erfc.
	if (y >= 2) {
		return -100.0;
	}
	else if (y <= 0) {
		return 100.0;
	}
	int zero_point = (y < 1);
	if (zero_point == 0) {
		y = 2 - y;
	}
	double t = sqrt(-2 * log(y / 2.));
	double x = -0.70711 * ((2.30753 + t * 0.27061) / (1. + t * (0.99229 + t * 0.04481)) - t);
	for (int i = 0; i < 2; i++) {
		double err = erfc(x) - y;
		x += err / (1.12837916709551257 * exp(-(x * x)) - x * err);
	}
	if (zero_point) {
		return x;
	}
	else {
		return -x;
	}

}

double Math::math_ppf(double x, double mu, double sigma)
{
	//The inverse function of cdf.
	return mu - sigma * sqrt(2) * math_erfcinv(2 * x);
}


