#include "stdafx.h"
#include "Gaussian.h"
#include <cmath>
#include <limits>
#include <iostream>
Gaussian::Gaussian(const double pi, const double tau)
{
	this->mu = tau / pi;
	this->sigma = sqrt(1 / pi);
	this->pi = pi;
	this->tau = tau;
}

Gaussian::~Gaussian()
{
}

const double Gaussian::get_mu() const
{
	if (this->pi) {
		return this->tau / this->pi;
	}
	else {
		return NULL;
	}
}

const double Gaussian::get_sigma() const
{
	if (this->pi) {
		return sqrt(1 / this->pi);
	}
	else {
		return INFINITY;
	}
}

const double Gaussian::get_pi() const
{
		return pi;

}

const double Gaussian::get_tau() const
{
	return tau;

}