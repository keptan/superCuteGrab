#include "trueskill.h"

static std::vector< std::vector< double > > players;

void addPlayer( double mu, double sigma, double team, double weight, double identifier );
bool validateTeams( std::vector< std::vector< double > > &T, int t );

/*
	Needed:
	
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
*/

/*
void SV_TSRate()
{
	if( Plugin_Scr_GetNumParam() < 2 )
	{
		Plugin_Scr_Error( "Usage: TS_Rate( (INT)<number of teams> ), (STRING)<team placements> \n" );
		return;
	}
	
	if( players.size() < 2 )
	{
		Plugin_Scr_Error( "Usage: Add atleast 2 players with TS_AddPlayer function.\n" );
		return;
	}
	
	int teams = Plugin_Scr_GetInt( 0 );
	
	if( !validateTeams( players, teams ) )
	{
		return;
	}
	
	char *placements = Plugin_Scr_GetString( 1 );
	char *p = strdup( placements );
	char *tok;
	std::vector< int > ranks;
	
	tok = strtok( p, " " );
	while( tok != NULL )
	{
		ranks.push_back( atoi( tok ) );
		tok = strtok( NULL, " " );
	}
	
	free( p );
	
	rate( players, ranks, teams );
	
	Plugin_Scr_MakeArray();
	for( unsigned int i = 0; i < players.size(); i++ )
	{
		Plugin_Scr_MakeArray();
		Plugin_Scr_AddFloat( ( float )players[ i ][ 0 ] );
		Plugin_Scr_AddArray();
		Plugin_Scr_AddFloat( ( float )players[ i ][ 1 ] );
		Plugin_Scr_AddArray();
		Plugin_Scr_AddArray();
	}
	
	players.clear();
}

void SV_TSQuality()
{
	if( Plugin_Scr_GetNumParam() < 1 )
	{
		Plugin_Scr_Error( "Usage: TS_Quality( (INT)<number of teams> ) \n" );
		return;
	}
	
	if( players.size() < 2 )
	{
		Plugin_Scr_Error( "Add atleast 2 players with TS_AddPlayer function  \n" );
		return;
	}
	
	int teams = Plugin_Scr_GetInt( 0 );
	
	if( !validateTeams( players, teams ) )
	{
		return;
	}
	
	double result = quality( players, teams );
	
	players.clear();
	Plugin_Scr_AddFloat( ( float )result );
}
*/

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

/*
void SV_TSAddPlayer()
{
	if( Plugin_Scr_GetNumParam() < 4 )
	{
		Plugin_Scr_Error( "Usage: TS_AddPlayer( (INT)<player ID>, (FLOAT)<player mu>, (FLOAT)<player sigma>, (INT)<player team>, (FLOAT)[<player weight>] )\n" );
		return;
	}
	
	float mu, sigma, weight;
	
	int identifier = Plugin_Scr_GetInt( 0 );
	mu = Plugin_Scr_GetFloat( 1 );
	sigma = Plugin_Scr_GetFloat( 2 );
	int team = Plugin_Scr_GetInt( 3 );
	
	if( Plugin_Scr_GetNumParam() == 5 )
		weight = Plugin_Scr_GetFloat( 4 );
	else
		weight = 1;
	
	addPlayer( ( double )mu, ( double )sigma, ( double )team, ( double )weight, ( double )identifier );
}

void SV_TSClearAllPlayers()
{
	players.clear();
}

*/
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

/*
void SV_TS_UpdateMu()
{
	if( Plugin_Scr_GetNumParam() < 1 )
	{
		Plugin_Scr_Error( "Usage: TS_UpdateMu( (FLOAT)<MU> )\n" );
		return;
	}
	
	float mu = Plugin_Scr_GetFloat( 0 );
	
	update_mu( ( double )mu );
}

void SV_TS_UpdateDraw()
{
	if( Plugin_Scr_GetNumParam() < 1 )
	{
		Plugin_Scr_Error( "Usage: TS_UpdateDraw( (FLOAT)<DRAW_CHANCE> ) \n" );
		return;
	}
	
	float draw = Plugin_Scr_GetFloat( 0 );
	
	if( draw < 0 || draw > 1 )
	{
		Plugin_Scr_Error( "<DRAW_CHANCE> must be between 0 and 1 \n" );
		return;
	}
	
	update_draw( ( double )draw );
}


PCL int OnInit()
{
	Plugin_ScrAddFunction( (char*)"TS_Quality", &SV_TSQuality );
	Plugin_ScrAddFunction( (char*)"TS_Rate", &SV_TSRate );
	Plugin_ScrAddFunction( (char*)"TS_AddPlayer", &SV_TSAddPlayer );
	Plugin_ScrAddFunction( (char*)"TS_ClearAllPlayers", &SV_TSClearAllPlayers );
	Plugin_ScrAddFunction( (char*)"TS_UpdateMu", &SV_TS_UpdateMu );
	Plugin_ScrAddFunction( (char*)"TS_UpdateDraw", &SV_TS_UpdateDraw );
	
    return 0;
}

PCL void OnInfoRequest(pluginInfo_t *info) 
{
	info->handlerVersion.major = PLUGIN_HANDLER_VERSION_MAJOR;
	info->handlerVersion.minor = PLUGIN_HANDLER_VERSION_MINOR;

	info->pluginVersion.major = 0;
	info->pluginVersion.minor = 1;	
	strncpy(info->fullName, "Trueskill plugin by Leiizko", sizeof(info->fullName));
	strncpy(info->shortDescription, "Game rating system.", sizeof(info->shortDescription));
	strncpy(info->longDescription, "This plugin is used to rank players according to their individual skill.", sizeof(info->longDescription));
}
*/
