#include <vector>
#include <cmath>
#include "matrix.h"

// INPUT: Vector matrix ( MUST BE SQUARE MATRIX )
// OUTPUT: Determinant as a double value
double determinant( std::vector< std::vector< double > > &A, int n )
{ 
	if ( n == 1 )
		return A[ 0 ][ 0 ];

	int i, sign = 1;
	double M = 0;
	std::vector< std::vector< double > > temp = initMatrix( n, n );

	for ( i = 0; i < n; i++ )
	{
		getCofactor( A, temp, 0, i, n );
		M += sign * A[ 0 ][ i ] * determinant( temp, n - 1 );

		sign = -sign;
	}

	return M;
}

void getCofactor( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &temp, int p, int q, int n )
{
	int i = 0, j = 0;
	for ( int row = 0; row < n; row++ )
	{
		for ( int col = 0; col < n; col++ )
		{
			if ( row != p && col != q )
			{
				temp[ i ][ j++ ] = A[ row ][ col ];

				if ( j == n - 1 )
				{
					j = 0;
					i++;
				}
			}
		}
	}
}

// Returns 1 if adjoint of matrix is actually 1
int adjugate( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &M )
{
	int n = A.size();
	if ( n == 1 )
	{
		return 1;
	}

	int i, j, sign = 1;
	std::vector< std::vector< double > > temp = initMatrix( n, n );

	for ( i = 0; i < n; i++ )
	{
		for ( j = 0; j < n; j++ )
		{
			getCofactor( A, temp, i, j, n );

			sign = ( ( i + j ) % 2 == 0 ) ? 1 : -1;

			M[ j ][ i ] = ( sign ) * ( determinant( temp, n - 1 ) );
		}
	}

	return 0;
}

// Returns 1 on faliure
int inverse( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &M )
{
	int n = A.size();
	double det = determinant( A, n );
	if ( det == 0 )
	{
		return 1;
	}

	if( n == 1 )
	{
		M[ 0 ][ 0 ] = pow( A[ 0 ][ 0 ], -1 );
		return 0;
	}

	int i, j;
	std::vector< std::vector< double > > adj = initMatrix( n, n );

	adjugate( A, adj );

	for ( i = 0; i < n; i++ )
		for ( j = 0; j < n; j++ )
			M[ i ][ j ] = adj[ i ][ j ] / det;

	return 0;
}

// INPUT: Vector matrix to transpose, number of rows and colums
// OUTPUT: Transposed vector matrix
std::vector< std::vector< double > > transpose( std::vector< std::vector< double > > &A )
{
	unsigned int i, j;

	std::vector< std::vector< double > > M = initMatrix( A[ 0 ].size(), A.size() );

	for ( i = 0; i < A[ 0 ].size(); i++ )
	{
		for ( j = 0; j <  A.size(); j++ )
		{
			M[ i ][ j ] = A[ j ][ i ];
		}
	}

	return M;
}

// INPUT: Number of rows and columns
// OUTPUT: Vector matrix with initiated 0 value
std::vector< std::vector< double > > initMatrix( int r, int c )
{
	std::vector< std::vector< double > > A( r, std::vector< double >( c, 0 ) );

	return A;
}

std::vector< std::vector< double > > matrixMultiply( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &B )
{
	std::vector< std::vector< double > > M = initMatrix( A.size(), B[ 0 ].size() );
	
	unsigned int i, j, k;
	for( i = 0; i < A.size(); i++ )
    {
        for( j = 0; j < B[ 0 ].size(); j++ )
        {
            for( k = 0; k < A[ 0 ].size(); k++ )
            {
                M[ i ][ j ] += A[ i ][ k ] * B[ k ][ j ];
            }
        }
    }
	return M;
}

std::vector< std::vector< double > > matrixMultiplyScalar( std::vector< std::vector< double > > &A, double S )
{
	std::vector< std::vector< double > > M = initMatrix( A.size(), A[ 0 ].size() );
	
	unsigned int i, j;
	for( i = 0; i < A.size(); i++ )
    {
        for( j = 0; j < A[ 0 ].size(); j++ )
        {
                M[ i ][ j ] += A[ i ][ j ] * S;
        }
    }
	return M;
}

std::vector< std::vector< double > > matrixAddition( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &B )
{
	std::vector< std::vector< double > > M = initMatrix( A.size(), A[ 0 ].size() );
	
	unsigned int i, j;
	for( i = 0; i < A.size(); i++ )
    {
        for( j = 0; j < A[ 0 ].size(); j++ )
        {
                M[ i ][ j ] += A[ i ][ j ] + B[ i ][ j ];
        }
    }
	return M;
}