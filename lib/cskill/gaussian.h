#pragma once

#include <cmath>
#include <limits>

class Gaussian
{
	double pi, tau, mu, sigma;

public:
	Gaussian();
	Gaussian( double pi, double tau );
	Gaussian( double mu, double sigma, double pi, double tau );
	double _pi();
	double _tau();
	double _mu();
	double _sigma();
	void setSigma( double pi );
	void setMu( double pi, double tau );

	Gaussian operator*( Gaussian *other );
	Gaussian operator/( Gaussian *other );
};