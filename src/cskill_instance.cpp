#include "cskill_instance.h"
#include <iostream> 

namespace cute {

CSkillInstance :: CSkillInstance (ScoreDB& s)
	: scores(s)
{}

void CSkillInstance :: generateTeams (const TagSet& player1, const TagSet& player2)
{
	team1 = player1 | player2;
	team2 = player2 | player1; 
	team3 = player1 & player2; 
}

void CSkillInstance :: eatVector (void)
{
	for (auto& p : players)
	{
		auto it = idTags.find(p[4]);
		if( it != idTags.end())
		{
			std::cout << "mu: " << p[0] << '\n';
			scores.insertScore( it->second, SkillDatum(p[0], p[1]));
		}
	}
}

void CSkillInstance :: addPlayer (double mu, double sigma, double team, double id)
{
	std::vector<double> p;
	p.resize(5);

	p[0] = mu;
	p[1] = sigma;
	p[2] = team;
	p[3] = 1;
	p[4] = id; 

	players.push_back(p);
}

void CSkillInstance :: addPlayer (const SkillDatum& t, int team, int id)
{

	std::vector<double> p;
	p.resize(5);

	p[0] = t.mu;
	p[1] = t.sigma;
	p[2] = team;
	p[3] = 1;
	p[4] = id; 


	players.push_back(p);
}

void CSkillInstance :: runScores (int winner)
{
	int idIt = 1;
	int naIt = -1;

	players.clear();
	idTags.clear();

	const auto averageMu = [&](const TagSet& t) -> double  
		{ 
			double a = 0;
			for(const auto& tag : t)
				a += scores.retrieveData(tag).mu;

			return a / t.size();
		};

	const auto averageSigma = [&](const TagSet& t) -> double 
	{
		double a = 0;
			for(const auto& tag : t)
				a += scores.retrieveData(tag).sigma;

			return a / t.size();
	};


	int padSize = std::max({team1.size(), team2.size(), team3.size()});

	for(const auto& t : team1)
	{
		idTags.insert(std::make_pair( idIt, t));
		addPlayer( scores.retrieveData(t), 1, idIt);
		std::cout << "retrieved: " << scores.retrieveData(t).mu << '\n';
		idIt++;
	}

	for(const auto& t : team2)
	{
		idTags.insert(std::make_pair( idIt, t));
		addPlayer( scores.retrieveData(t), 2, idIt);
		std::cout << "retrieved: " << scores.retrieveData(t).mu << '\n';
		idIt++;
	}

	for(const auto& t : team3)
	{
		idTags.insert(std::make_pair( idIt, t));
		addPlayer( scores.retrieveData(t), 3, idIt);
		idIt++;
	}

	for(int i = team1.size(); i < padSize; i++)
	{
		addPlayer( averageMu(team1), 33, 1, naIt);
		naIt--;
	}

	for(int i = team2.size(); i < padSize; i++)
	{
		addPlayer( averageMu(team2), 33, 2, naIt);
		naIt--;
	}

	for(int i = team3.size(); i < padSize; i++)
	{
		addPlayer( averageMu(team3), 33, 3, naIt);
		naIt--;
	}
	
	std::vector<int> ranks; 

	if(team3.size())
	{
		if(winner == 3) ranks = {0,0,0};
		if(winner == 1) ranks = {0,2,1};
		if(winner == 2) ranks = {2,0,1};

	}
	else
	{
		if(winner == 3) ranks = {0,0};
		if(winner == 1) ranks = {0,1};
		if(winner == 2) ranks = {1,0};
	}

		int teams = team3.size() == 0 ? 2 : 3;

		for(auto& t: team1)
		{
			std::cout << t.tag << '\n';
		}

		for(auto& t: team2)
		{
			std::cout << t.tag << '\n';
		}

		for(auto& t: team3)
		{
			std::cout << t.tag << '\n';
		}

		if (team1.size() == 0 || team2.size() == 0)
			return;

		rate(players, ranks, teams);
		eatVector();
}



}





