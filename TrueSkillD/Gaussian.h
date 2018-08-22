#ifndef  GAUSSIAN_INC
#define  GAUSSIAN_INC
#pragma once
class Gaussian
{
public:
	Gaussian(const double pi, const double tau);
	~Gaussian();
	const double get_mu() const;
	const double get_sigma() const;
	const double get_pi() const;
	const double get_tau() const;
protected:
	double pi;
	double tau;
	double mu;
	double sigma;
};

#endif