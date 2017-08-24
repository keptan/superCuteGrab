#include <cmath>
#ifndef M_PI
	static const double M_PI = 3.14159265358979323846;
#endif

double erfcinv( double y );

// Probability density function
double pdf( double x, double mu, double sigma )
{
	return ( 1 / sqrt( 2 * M_PI ) * std::abs( sigma ) * exp( -pow( ( x - mu ) / std::abs( sigma ), 2 ) / 2 ) );
}

// Cumulative distribution function
double cdf( double x, double mu, double sigma )
{
	return 0.5 * erfc( -( x - mu ) / ( sigma * sqrt( 2 ) ) );
}

// Inverse cdf
double ppf( double x, double mu, double sigma )
{
	return mu - sigma * sqrt( 2 ) * erfcinv( 2 * x );
}

// Inverse ercf ( error complementary function )
double erfcinv( double y )
{
	if( y >= 2 )
		return -100;
	else if( y <= 0 )
		return 100;
	
	bool zero_point = y < 1;
	if( !zero_point )
		y = 2 - y;

	double t = sqrt( -2 * log( y / 2 ) );
	double x = -0.70711 * ( ( 2.30753 + t * 0.27061 ) / ( 1 + t * ( 0.99229 + t * 0.04481 ) ) - t );
	for( int i = 0; i < 2; i++ )
	{
		double err = erfc( x ) - y;
		x += err / ( 1.12837916709551257 * exp( -( pow( x, 2 ) ) ) - x * err );
	}
	
	return zero_point ? x : -x;
}