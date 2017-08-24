#include "gaussian.h"

static const double INF = std::numeric_limits<double>::infinity();

Gaussian::Gaussian()
{
	this->mu = 0;
	this->sigma = INF;
	this->pi = 0;
	this->tau = 0;
}

Gaussian::Gaussian( double pi, double tau )
{
	setMu( pi, tau );
	setSigma( pi );
	this->pi = pi;
	this->tau = tau;
}

Gaussian::Gaussian( double mu, double sigma, double pi, double tau )
{
	this->mu = mu;
	this->sigma = sigma;
	this->pi = pow( this->sigma, -2 );
	this->tau = this->pi * this->mu;
}

Gaussian Gaussian::operator*( Gaussian *other )
{
	double pi, tau;
	pi = this->pi + other->pi;
	tau = this->tau + other->tau;
	Gaussian result = Gaussian( pi, tau );
	return result;
}

Gaussian Gaussian::operator/( Gaussian *other )
{
	double pi, tau;
	pi = this->pi - other->pi;
	tau = this->tau - other->tau;
	Gaussian result = Gaussian( pi, tau );
	return result;
}

void Gaussian::setMu( double pi, double tau )
{
	this->mu = pi ? tau / pi : pi;
}

void Gaussian::setSigma( double pi )
{
	this->sigma = pi ? sqrt( 1 / pi ) : INF;
}

double Gaussian::_pi() { return this->pi; };
double Gaussian::_tau() { return this->tau; };
double Gaussian::_mu() { return this->mu; };
double Gaussian::_sigma() { return this->sigma; };