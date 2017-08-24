#include "factorgraph.h"

static const double INF = std::numeric_limits<double>::infinity();

double Variable::update_value( Factor *factor, double pi, double tau )
{
	Gaussian value = Gaussian( pi, tau );
	Gaussian old_message = this->messages[ *factor ];
	Gaussian multi = value * &old_message;
	Gaussian div = multi / &this->value;
	attach_factor( factor, &div );

	return set( &value );
}

double Variable::update_value( Factor *factor, Gaussian *value )
{
	Gaussian old_message = this->messages[ *factor ];
	Gaussian multi = *value * &old_message;
	Gaussian div = multi / &this->value;
	attach_factor( factor, &div );

	return set( value );
}

double Variable::update_message( Factor *factor, double pi, double tau )
{
	Gaussian message = Gaussian( pi, tau );
	Gaussian old_message = this->messages[ *factor ];
	attach_factor( factor, &message );
	Gaussian div = this->value / &old_message;
	Gaussian multi = div * &message;
	return set( &multi );
}

double Variable::update_message( Factor *factor, Gaussian *message )
{
	Gaussian old_message = this->messages[ *factor ];
	attach_factor( factor, message );
	Gaussian div = this->value / &old_message;
	Gaussian multi = div * message;

	return set( &multi );
}

Gaussian Variable::operator[]( Factor *factor )
{
	return this->messages[ *factor ];
}

void Variable::attach_factor( Factor *factor, Gaussian *message )
{
	this->messages[ *factor ] = *message;
}

double Variable::delta( Gaussian *other )
{
	double pi_delta = std::abs( this->value._pi() - other->_pi() );

	if ( pi_delta == INF )
		return 0;

	return std::max( std::abs( this->value._tau() - other->_tau() ), std::sqrt( pi_delta ) );
}

double Variable::set( Gaussian *other )
{
	double delta = this->delta( other );
	this->value = Gaussian( other->_pi(), other->_tau() );

	return delta;
}

Gaussian Variable::getVal()
{
	return this->value;
}

int Factor::s_id;

void Factor::initF( std::vector< Variable* > &vars )
{
	this->vars = vars;
	for ( unsigned int i = 0; i < vars.size(); i++ )
	{
		Gaussian msg = Gaussian();
		vars[ i ]->attach_factor( this, &msg );
	}
}

std::vector< Variable* > Factor::getVar()
{
	return this->vars;
}

PriorFactor::PriorFactor( Variable *var, Gaussian *val, double dynamic )
{
	std::vector< Variable* > vars;
	vars.push_back( var );
	initF( vars );
	this->val = *val;
	this->dynamic = dynamic;
}

double PriorFactor::down()
{
	double sigma = std::sqrt( std::pow( this->val._sigma(), 2 ) + std::pow( this->dynamic, 2 ) );
	Gaussian value = Gaussian( this->val._mu(), sigma, 0, 0 );
	return ( this->vars )[ 0 ]->update_value( this, &value );
}

LikelihoodFactor::LikelihoodFactor( Variable *mean_var, Variable *value_var, double variance )
{
	std::vector< Variable* > vars;
	vars.push_back( mean_var );
	vars.push_back( value_var );
	initF( vars );

	this->mean = mean_var;
	this->value = value_var;
	this->variance = variance;
}

double LikelihoodFactor::calc_a( Gaussian *val )
{
	return 1.0 / ( 1.0 + this->variance * val->_pi() );
}

double LikelihoodFactor::down()
{
	Gaussian by = (*this->mean )[this];
	Gaussian msg = this->mean->getVal() / &by;
	double a = calc_a( &msg );
	return this->value->update_message( this, a * msg._pi(), a * msg._tau() );
}

double LikelihoodFactor::up()
{
	Gaussian by = ( *this->value )[ this ];
	Gaussian msg = this->value->getVal() / &by;
	double a = calc_a( &msg );
	return this->mean->update_message( this, a * msg._pi(), a * msg._tau() );
}

SumFactor::SumFactor( Variable *sum, std::vector< Variable* > &terms, std::vector< double > &coeffs )
{
	std::vector< Variable* > vars;
	vars.push_back( sum );
	for ( unsigned int i = 0; i < terms.size(); i++ )
	{
		vars.push_back( terms[ i ] );
	}

	initF( vars );
	this->sum = sum;
	this->terms = terms;
	this->coeffs = coeffs;
}

double SumFactor::down()
{
	std::vector< Gaussian > msgs;
	for ( unsigned int i = 0; i < this->terms.size(); i++ )
	{
		msgs.push_back( ( *this->terms[ i ] )[ this ] );
	}

	return update( this->sum, this->terms, msgs, this->coeffs );
}

double SumFactor::up( unsigned int index )
{
	double coeff = this->coeffs[ index ];
	std::vector< double > coeffs;
	for ( unsigned int i = 0; i < this->coeffs.size(); i++ )
	{
		if ( coeff == 0.0 )
		{
			coeffs.push_back( 0 );
			continue;
		}
		if ( i == index )
		{
			coeffs.push_back( 1.0 / coeff );
		}
		else
		{
			coeffs.push_back( ( this->coeffs[ i ] / coeff ) * -1 );
		}
	}

	std::vector< Variable* > vals;
	for ( unsigned int i = 0; i < this->terms.size(); i++ )
	{
		if ( i == index )
		{
			vals.push_back( this->sum );
		}
		else
		{
			vals.push_back( this->terms[ i ] );
		}
	}
	std::vector< Gaussian > msgs;
	for ( unsigned int i = 0; i < vals.size(); i++ )
	{
		msgs.push_back( ( *vals[ i  ] )[ this ] );
	}

	return update( this->terms[ index ], vals, msgs, coeffs );
}

double SumFactor::update( Variable *var, std::vector< Variable* > &vals, std::vector< Gaussian > &msgs, std::vector< double > coeffs )
{
	double pi_inv = 0, mu = 0;

	for ( unsigned int i = 0; i < vals.size(); i++ )
	{
		Gaussian div;
		div = vals[ i ]->getVal() / &msgs[ i ];
		mu += coeffs[ i ] * div._mu();
		if ( pi_inv == INF )
		{
			continue;
		}
		if ( !div._pi() )
		{
			pi_inv = INF;
		}
		else
		{
			pi_inv += std::pow( coeffs[ i ], 2 ) / div._pi();
		}
	}

	double pi = 1.0 / pi_inv;
	double tau = pi * mu;
	return var->update_message( this, pi, tau );
}

TruncateFactor::TruncateFactor( Variable *var, std::function< double( double, double ) > v_func, std::function< double( double, double ) > w_func, double draw_margin )
{
	std::vector< Variable* > vars;
	vars.push_back( var );
	initF( vars );

	this->v_func = v_func;
	this->w_func = w_func;
	this->draw_margin = draw_margin;
}

double TruncateFactor::up()
{
	Variable *val = this->vars[ 0 ];
	Gaussian msg = (*this->vars[ 0 ])[ this ];
	Gaussian div = val->getVal() / &msg;
	double sqrt_pi = std::sqrt( div._pi() );
	double v = this->v_func( div._tau() / sqrt_pi, this->draw_margin * sqrt_pi );
	double w = this->w_func( div._tau() / sqrt_pi, this->draw_margin * sqrt_pi );
	double denom = ( 1.0 - w );
	double pi = div._pi() / denom;
	double tau = ( div._tau() + sqrt_pi * v ) / denom;

	return val->update_value( this, pi, tau );
}