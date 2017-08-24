#pragma once

#include <algorithm>
#include <map>
#include <vector>
#include <limits>
#include <functional>
#include "gaussian.h"

class Variable;

class Factor
{
	static int s_id;
protected:
	std::vector< Variable* > vars;
public:
	int id;
	Factor() : id( s_id++ ) {};
	void initF( std::vector< Variable* > &vars );
	std::vector< Variable* > getVar();
};

struct FactorKeyMapper 
{
	bool operator()( const Factor& lhs, const Factor& rhs ) const 
	{
		return lhs.id < rhs.id;
	}
};

class Variable
{
	Gaussian value;
	std::map< Factor, Gaussian, FactorKeyMapper > messages;

public:
	Variable() : value(Gaussian()) {};
	double delta( Gaussian *other );
	double set( Gaussian *other );
	double update_message( Factor *factor, double pi, double tau );
	double update_message( Factor *factor, Gaussian *message );
	double update_value( Factor *factor, double pi, double tau );
	double update_value( Factor *factor, Gaussian *value );
	Gaussian operator[]( Factor *factor );
	void attach_factor( Factor *factor, Gaussian *message );
	Gaussian getVal();
};

class PriorFactor : public Factor
{
	double dynamic;
	Gaussian val;

public:
	PriorFactor( Variable *var, Gaussian *val, double dynamic );
	double down();
};

class LikelihoodFactor : public Factor
{
	Variable *value;
	Variable *mean;
	double variance;

public:
	LikelihoodFactor( Variable *mean_var, Variable *value_var, double variance );
	double calc_a( Gaussian *val );
	double down();
	double up();
};

class SumFactor : public Factor
{
	Variable *sum;
	std::vector< Variable* > terms;
	std::vector< double > coeffs;

public:
	SumFactor( Variable *sum, std::vector< Variable* > &terms, std::vector< double > &coeffs );
	double down();
	double up( unsigned int index );
	double update( Variable *var, std::vector< Variable* > &vals, std::vector< Gaussian > &msgs, std::vector< double > coeffs );
};

class TruncateFactor : public Factor
{
	std::function< double( double, double ) > v_func;
	std::function< double( double, double ) > w_func;
	double draw_margin;

public:
	TruncateFactor( Variable *var, std::function< double( double, double ) > v_func, std::function< double( double, double ) > w_func, double draw_margin );
	double up();
};