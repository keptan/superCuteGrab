#pragma once

#include <cmath>
#include <vector>
#include <functional>
#include "matrix.h"
#include "gaussian.h"
#include "factorgraph.h"

// Trueskill
double quality( std::vector< std::vector< double > > &T, int t );
void sortTeams( std::vector< std::vector< double > > &T, int t );
void rate( std::vector< std::vector< double > > &T, std::vector< int > &R, int t, double min_delta = 0.0001 );
void sortTeamsByRank( std::vector< std::vector< double > > &T, std::vector< int > &R );
std::vector< int > getTeamSize( std::vector< std::vector< double > > &T, int t );
double v_win( double diff, double draw_margin );
double v_draw( double diff, double draw_margin );
double w_win( double diff, double draw_margin );
double w_draw( double diff, double draw_margin );
double calc_draw_margin( double draw_probability, int size );
void validateWeights( std::vector< std::vector< double > > &T );
void update_mu( double new_mu );
void update_draw( double new_draw );

// BaseMath
double pdf( double x, double mu = 0, double sigma = 1 );
double cdf( double x, double mu = 0, double sigma = 1 );
double ppf( double x, double mu = 0, double sigma = 1 );