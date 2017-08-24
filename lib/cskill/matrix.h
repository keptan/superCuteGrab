#pragma once

double determinant( std::vector< std::vector< double > > &A, int n );
void getCofactor( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &temp, int p, int q, int n );
int adjugate( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &M );
int inverse( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &M );
std::vector< std::vector< double > > transpose( std::vector< std::vector< double > > &A );
std::vector< std::vector< double > > initMatrix( int r, int c );
std::vector< std::vector< double > > matrixAddition( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &B );
std::vector< std::vector< double > > matrixMultiplyScalar( std::vector< std::vector< double > > &A, double S );
std::vector< std::vector< double > > matrixMultiply( std::vector< std::vector< double > > &A, std::vector< std::vector< double > > &B );