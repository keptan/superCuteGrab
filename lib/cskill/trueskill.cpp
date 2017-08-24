#include "trueskill.h"

// TODO:
//      Dynamic draw probability

// updates the TAU and BETA params
// update_mu( double new_mu )
static double MU = 25;
static double TAU = MU / 3 / 100;
static double BETA = MU / 3 / 2;
// updates DRAW_PROBABILITY param
// update_draw( double new_draw )
static double DRAW_PROBABILITY = 0.1;

// INPUT: T = Vector matrix of teams and trueskill properties
//        T[ i ][ 0 ] = mu
//        T[ i ][ 1 ] = sigma
//        T[ i ][ 2 ] = team ( team 1 = 1 | team 2 = 2 | team 3 = 3 |..... )
//        T[ i ][ 3 ] = Weight
//        T[ i ][ 4 ] = Identifier
//        t = total number of teams
// OUTPUT: Quality of the game
double quality( std::vector< std::vector< double > > &T, int t )
{
	validateWeights( T );
	sortTeams( T, t );
	
	t--;
	if( !t )
		return 0;
	
	int i, j, n = T.size();
	
	std::vector< std::vector< double > > mean_matrix = initMatrix( n, 1 );
	std::vector< std::vector< double > > variance_matrix = initMatrix( n, n );
	std::vector< std::vector< double > > rotated_a_matrix = initMatrix( t, n );
	std::vector< std::vector< double > > a_matrix = initMatrix( t, n );
	std::vector< std::vector< double > > _ata = initMatrix( t, t );
	std::vector< std::vector< double > > _atsa = initMatrix( t, t );
	std::vector< std::vector< double > > start = initMatrix( 1, t );
	std::vector< std::vector< double > > middle = initMatrix( t, t );
	std::vector< std::vector< double > > middleinv = initMatrix( t, t );
	std::vector< std::vector< double > > end = initMatrix( t, 1 );
	std::vector< std::vector< double > > ra_matrix = initMatrix( t, n );
	std::vector< std::vector< double > > rv_matrix = initMatrix( t, n );
	std::vector< std::vector< double > > mean_matrix_transposed = initMatrix( n, 1 );
	std::vector< std::vector< double > > start_middleinv = initMatrix( t, t );
	std::vector< std::vector< double > > start_middleinv_end = initMatrix( t, 1 );
	std::vector< std::vector< double > > start_middleinv_end_scalar = initMatrix( t, 1 );
	
	// Mu
	for ( i = 0; i < n; i++ )
	{
		mean_matrix[ i ][ 0 ] = T[ i ][ 0 ];
	}
	
	// Sigma ^ 2
	for ( i = 0; i < n; i++ )
	{
		variance_matrix[ i ][ i ] = pow( T[ i ][ 1 ], 2 );
	}
	
    //    1     2     2      3     4
    // | 0.00  0.00  0.00  1.00 -1.00 |
    // | 0.00  1.00  1.00 -1.00  0.00 |
    // | 1.00 -1.00 -1.00  0.00  0.00 |
	// Teams
	for( i = 0; i < t; i++ )
	{
		for( j = 0; j < n; j++ )
		{
			if ( T[ j ][ 2 ] < t - i )
			{
				continue;
			}
			else if ( T[ j ][ 2 ] > t - i + 1 )
			{
				break;
			}
			
			if ( T[ j ][ 2 ] == t - i )
			{
				rotated_a_matrix[ i ][ j ] = 1 * T[ j ][ 3 ];
			}
			else
			{
				rotated_a_matrix[ i ][ j ] = -1 * T[ j ][ 3 ];
			}
		}
	}
	
	a_matrix = transpose( rotated_a_matrix );
	ra_matrix = matrixMultiply( rotated_a_matrix, a_matrix );
	rv_matrix = matrixMultiply( rotated_a_matrix, variance_matrix );
	mean_matrix_transposed = transpose( mean_matrix );
	_ata = matrixMultiplyScalar( ra_matrix, pow( BETA, 2 ) );
	_atsa = matrixMultiply( rv_matrix, a_matrix );
	start =  matrixMultiply( mean_matrix_transposed, a_matrix );
	middle = matrixAddition( _ata, _atsa );
	end = matrixMultiply( rotated_a_matrix, mean_matrix );
	int check = inverse( middle, middleinv );
	
	// Matrix inversion failed, cannot compute quality
	if( check )
		return 0;
	
	start_middleinv = matrixMultiply( start, middleinv );
	start_middleinv_end = matrixMultiply( start_middleinv, end );
	start_middleinv_end_scalar = matrixMultiplyScalar( start_middleinv_end, -0.5 );

	double e_arg = determinant( start_middleinv_end_scalar, start_middleinv_end_scalar.size() );
	double s_arg = determinant( _ata, _ata.size() ) / determinant( middle, middle.size() );

	return exp( e_arg ) * sqrt( s_arg );
}

// INPUT: T = Vector matrix of teams and trueskill properties
//        T[ i ][ 0 ] = mu
//        T[ i ][ 1 ] = sigma
//        T[ i ][ 2 ] = team ( team 1 = 1 | team 2 = 2 | team 3 = 3 |..... )
//        T[ i ][ 3 ] = Weight
//        T[ i ][ 4 ] = Identifier
//        R = Vector array of team ranks = index = team number - 1
//        Lower value is better (win), same value is draw
//        Example: Team 2 and team 3 draw, team 1 loses
//        R[0] = 1 ; R[1] = 0 ; R[2] = 0
//        Example: Team 2 wins, team 1 loses
//        R[0] = 1 ; R[1] = 0
//        t = total number of teams
//        Optional param min_delta = Loop will stop once delta is lower than this param. default: 0.0001
// OUTPUT: Recalculated player ratings in the same order as inputed
void rate( std::vector< std::vector< double > > &T, std::vector< int > &R, int t, double min_delta )
{
	validateWeights( T );
	std::vector< std::vector< double > > org;
	org = T;
	sortTeamsByRank( T, R );
	
	for ( unsigned int i = 0; i < R.size(); i++ )
	{
		for ( unsigned int n = i + 1; n < R.size(); n++ )
		{
			if ( R[ i ] > R[ n ] )
			{
				int temp = R[ i ];
				R[ i ] = R[ n ];
				R[ n ] = temp;
			}
		}
	}
	
	unsigned int size = T.size(), index = 0;
	double ident = T[ 0 ][ 2 ];
	std::vector< std::vector< Gaussian > > ratings( t );

	for ( unsigned int i = 0; i < size; i++ )
	{
		if ( T[ i ][ 2 ] != ident )
		{
			ident = T[ i ][ 2 ];
			index++;
		}

		ratings[ index ].push_back( Gaussian( T[ i ][ 0 ], T[ i ][ 1 ], 0, 0 ) );
	}

	size = T.size();
	std::vector< Variable > rating_vars( size, Variable() );
	std::vector< Variable > perf_vars( size, Variable() );
	std::vector< Variable > team_perf_vars( t, Variable() );
	std::vector< Variable > team_diff_vars( t - 1, Variable() );
	std::vector< int > team_sizes = getTeamSize( T, t );

	std::vector< PriorFactor > build_rating_layer;
	std::vector< LikelihoodFactor > build_perf_layer;
	for ( unsigned int i = 0; i < size; i++ )
	{
		Gaussian rating = Gaussian( T[ i ][ 0 ], T[ i ][ 1 ], 0, 0 );
		build_rating_layer.push_back( PriorFactor( &rating_vars[ i ], &rating, TAU ) );
		build_perf_layer.push_back( LikelihoodFactor( &rating_vars[ i ], &perf_vars[ i ], std::pow( BETA, 2 ) ) );
	}

	std::vector< SumFactor > build_team_perf_layer;
	std::vector< Variable* > child_perf_vars;
	std::vector< double > coeffs;
	int start, end;
	for ( int i = 0; i < t; i++ )
	{
		i > 0 ? start = team_sizes[ i - 1 ] : start = 0;
		end = team_sizes[ i ];
		for ( int n = start; n < end; n++ )
		{
			child_perf_vars.push_back( &perf_vars[ n ] );
			coeffs.push_back( T[ n ][ 3 ] );
		}

		build_team_perf_layer.push_back( SumFactor( &team_perf_vars[ i ], child_perf_vars, coeffs ) );
		child_perf_vars.clear();
		coeffs.clear();
	}

	for ( unsigned int i = 0; i < build_rating_layer.size(); i++ )
	{
		build_rating_layer[ i ].down();
	}

	for ( unsigned int i = 0; i < build_perf_layer.size(); i++ )
	{
		build_perf_layer[ i ].down();
	}

	for ( unsigned int i = 0; i < build_team_perf_layer.size(); i++ )
	{
		build_team_perf_layer[ i ].down();
	}

	std::vector< SumFactor > build_team_diff_layer;
	std::function< double( double, double ) > v_func, w_func;
	int calcsize;
	double draw_margin;
	coeffs.push_back( 1 );
	coeffs.push_back( -1 );
	for ( int i = 0; i < t - 1; i++ )
	{
		for ( int n = i; n < i + 2; n++ )
		{
			child_perf_vars.push_back( &team_perf_vars[ n ] );
		}
		build_team_diff_layer.push_back( SumFactor( &team_diff_vars[ i ], child_perf_vars,  coeffs ) );
		child_perf_vars.clear();
	}
	coeffs.clear();

	std::vector< TruncateFactor > build_trunc_layer;
	for ( int i = 0; i < t - 1; i++ )
	{
		calcsize = 0;
		for ( int n = i; n < i + 2; n++ )
		{
			calcsize += ratings[ n ].size();
		}
		draw_margin = calc_draw_margin( DRAW_PROBABILITY, calcsize );
		if ( R[ i ] == R[ i + 1 ] )
		{
			v_func = v_draw;
			w_func = w_draw;
		}
		else
		{
			v_func = v_win;
			w_func = w_win;
		}

		build_trunc_layer.push_back( TruncateFactor( &team_diff_vars[ i ], v_func, w_func, draw_margin ) );
	}

	unsigned int team_diff_len = build_team_diff_layer.size();
	double delta;
	for ( int i = 0; i < 10; i++ )
	{
		if ( team_diff_len == 1 )
		{
			build_team_diff_layer[ 0 ].down();
			delta = build_trunc_layer[ 0 ].up();
		}
		else
		{
			delta = 0;
			for ( unsigned int n = 0; n < team_diff_len - 1; n++ )
			{
				build_team_diff_layer[ n ].down();
				delta = std::max( delta, build_trunc_layer[ n ].up() );
				build_team_diff_layer[ n ].up( 1 );
			}
			for ( unsigned int n = team_diff_len - 1; n > 0; n-- )
			{
				build_team_diff_layer[ n ].down();
				delta = std::max( delta, build_trunc_layer[ n ].up() );
				build_team_diff_layer[ n ].up( 0 );
			}
		}
		if ( delta <= min_delta )
		{
			break;
		}
	}

	build_team_diff_layer[ 0 ].up( 0 );
	build_team_diff_layer[ team_diff_len - 1 ].up( 1 );
	for ( unsigned int i = 0; i < build_team_perf_layer.size(); i++ )
	{
		for ( unsigned int n = 0; n < build_team_perf_layer[ i ].getVar().size() - 1; n++ )
		{
			build_team_perf_layer[ i ].up( n );
		}
	}

	for ( unsigned int i = 0; i < build_perf_layer.size(); i++ )
	{
		build_perf_layer[ i ].up();
	}

	for ( unsigned int i = 0; i < T.size(); i++ )
	{
		T[ i ][ 0 ] = rating_vars[ i ].getVal()._mu();
		T[ i ][ 1 ] = rating_vars[ i ].getVal()._sigma();
	}

	for ( unsigned int i = 0; i < T.size(); i++ )
	{
		for ( unsigned int n = 0; n < T.size(); n++ )
		{
			if ( org[ i ][ 4 ] == T[ n ][ 4 ] )
			{
				org[ i ][ 0 ] = T[ n ][ 0 ];
				org[ i ][ 1 ] = T[ n ][ 1 ];
				break;
			}
		}
	}
	T = org;
}

void sortTeamsByRank( std::vector< std::vector< double > > &T, std::vector< int > &R )
{
	unsigned int i, n, idx;
	std::vector< std::vector< double > > sorted;
	std::vector< int > ranks_temp = R;
	std::vector< int > ranks_sorted( R.size(), 999 );


	for ( i = 0; i < R.size(); i++ )
	{
		idx = -1;
		for ( n = 0; n < ranks_temp.size(); n++ )
		{
			if ( ranks_temp[ n ] < ranks_sorted[ i ] )
			{
				ranks_sorted[ i ] = ranks_temp[ n ];
				idx = n;
			}
		}
		ranks_sorted[ i ] = idx + 1;
		ranks_temp[ idx ] = 999;
	}

	for ( i = 0; i < R.size(); i++ )
	{
		for ( n = 0; n < T.size(); n++ )
		{
			if ( ( int )T[ n ][ 2 ] == ranks_sorted[ i ] )
			{
				sorted.push_back( T[ n ] );
			}
		}
	}

	T = sorted;
}

void validateWeights( std::vector< std::vector< double > > &T )
{
	for ( unsigned int i = 0; i < T.size(); i++ )
	{
		if ( T[ i ][ 3 ] <= 0 )
		{
			T[ i ][ 3 ] = 0.001;
		}
	}
}

std::vector< int > getTeamSize( std::vector< std::vector< double > > &T, int t )
{
	int count = 0;
	unsigned int n, i = 0, size = T.size();
	double ident = T[ 0 ][ 2 ];
	std::vector< int > team_size( t );
	for ( n = 0; n < size; n++ )
	{
		if ( T[ n ][ 2 ] != ident )
		{
			ident = T[ n ][ 2 ];
			team_size[ i ] = count;
			i++;
		}
		count++;
	}

	team_size[ i ] = count;

	return team_size;
}

void sortTeams( std::vector< std::vector< double > > &T, int t )
{
	std::vector< std::vector< double > > A = initMatrix( T.size(), 5 );
	unsigned int n, j = 0;

	for ( int i = 1; i <= t; i++ )
	{
		for ( n = 0; n < T.size(); n++ )
		{
			if ( ( int )T[ n ][ 2 ] == i )
			{
				A[ j ] = T[ n ];
				j++;
			}
		}
	}

	T = A;
}

double v_win( double diff, double draw_margin )
{
	double x = diff - draw_margin;
	double denom = cdf( x );

	return denom ? ( pdf( x ) / denom ) : -x;
}

double v_draw( double diff, double draw_margin )
{
	double abs_diff = std::abs( diff );
	double a = draw_margin - abs_diff;
	double b = -draw_margin - abs_diff;
	double denom = cdf( a ) - cdf( b );
	double numer = pdf( b ) - pdf( a );
	return (denom ? ( numer / denom ) : a) * (diff < 0 ? -1 : 1);
}

double w_win( double diff, double draw_margin )
{
	double x = diff - draw_margin;
	double v = v_win( diff, draw_margin );
	double w = v * ( v + x );
	
	if ( 0 < w && w < 1 )
	{
		return w;
	}
	return -1;
}

double w_draw( double diff, double draw_margin )
{
	double abs_diff = std::abs( diff );
	double a = draw_margin - abs_diff;
	double b = -draw_margin - abs_diff;
	double denom = cdf( a ) - cdf( b );
	if ( !denom )
	{
		return -1;
	}

	double v = v_draw( abs_diff, draw_margin );
	return std::pow( v, 2 ) + ( a * pdf( a ) - b * pdf( b ) ) / denom;;
}

double calc_draw_margin( double draw_probability, int size )
{
	return ppf( ( draw_probability + 1 ) / 2 ) * std::sqrt( size ) * BETA;
}

void update_mu( double new_mu )
{
	MU = new_mu;
	TAU = MU / 3 / 100;
	BETA = MU / 3 / 2;
}

void update_draw( double new_draw )
{
	DRAW_PROBABILITY = new_draw;
}