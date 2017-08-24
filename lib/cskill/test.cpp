#include "trueskill.h"
#include <iostream>

static std::vector< std::vector< double > > players;

//void rate( std::vector< std::vector< double > > &T, std::vector< int > &R, int t, double min_delta = 0.0001 );



bool validateTeams( std::vector< std::vector< double > > &T, int t )
{
	if( t < 2 )
	{
		return false;
	}
	
	for( unsigned int i = 0; i < T.size(); i++ )
	{
		if( ( int )T[ i ][ 2 ] > t )
		{
			return false;
		}
	}
	
	return true;
}

void addPlayer( double mu, double sigma, double team, double weight, double identifier )
{
	unsigned int n = players.size();

	players.resize( n + 1 );
	players[ n ].resize( 5 );
	players[ n ][ 0 ] = mu;
	players[ n ][ 1 ] = sigma;
	players[ n ][ 2 ] = team;
	players[ n ][ 3 ] = weight;
	players[ n ][ 4 ] = identifier;
}



class skillclass
{
	private:
		double mu = 200;
		double sigma = 33;
		int team = 1;
		double weight = 1;
		double identifier = 6;
		std::vector<double> playerVector;

	public:
		double getMu()
		{
			return mu;
		}
		double getSigma()
		{
			return sigma;
		}

		std::vector<double> buildVector()
		{
			playerVector.push_back(mu);
			playerVector.push_back(sigma);
			playerVector.push_back(team);
			playerVector.push_back(identifier);

			return playerVector;
		}

		void printVector()
		{
			for(auto n : playerVector)
					std::cout<<n<<' ';

			std::cout<<std::endl;
		}

	};

	
		

	int main()
{
	addPlayer(100,33,1,1,0);
	addPlayer(100,33,1,1,1);
	addPlayer(100,33,1,1,2);

	addPlayer(100,33,2,1,3);
	addPlayer(100,33,2,1,4);
	addPlayer(100,33,2,1,5);

	skillclass sc;
	players.push_back(sc.buildVector());


	for(auto p:players)
		for(auto l : p)
			std::cout<<l<<" ";
	std::cout<<std::endl;

	std::vector< int > ranks;
	ranks.push_back(0);
	ranks.push_back(1);

	int teams = 2;

	validateTeams(players,2);

	rate(players,ranks,teams);

	for(auto p:players)
		for(auto l : p)
			std::cout<<l<<" ";
	std::cout<<std::endl;

	sc.printVector();






}
