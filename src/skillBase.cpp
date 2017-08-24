#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>


#include "cskill/trueskill.h"
#include "skillBase.h"

namespace cute
{
	SkillDatum :: SkillDatum(std::string n,double m, double s, double c )
		:mu(m),sigma(s),tagName(n),count(c)
	{}
	
	std::string SkillDatum :: getName()
	{
		return tagName;
	}

	double SkillDatum :: getMu()
	{
		return mu;
	}

	double SkillDatum :: getSigma()
	{
		return sigma;
	}

	int SkillDatum :: getCount()
	{
		return count;
	}
	int SkillDatum :: getTeam()
	{
		return team;
	}

	void SkillDatum :: setTeam(int t)
	{
		team = t;
	}

	void SkillDatum :: setMu(double  m)
	{
		mu = m;
	}

	void SkillDatum :: setSigma(double s)
	{
		sigma = s;
	}

	void SkillDatum :: iterateCount()
	{
		count += 1;
	}

	int SkillDatum :: getId()
	{
		return id;
	}

	void SkillDatum :: setId(int i)
	{
		id = i;
	}

}
namespace cute
{
	SkillBase :: SkillBase(std::string l):loc(l)
	{
		readFile();
	}

	bool SkillBase :: hasTag(std::string n)
	{
	std::fstream db;
		db.open(loc);


		db.clear();
		db.seekg(0,std::ios::beg);

		std::istringstream is;
		for(auto s: localTags)
			if(s.getName() == n){
				db.close();
				return true;
			}

		db.close();
		return false;
	}
	
	bool SkillBase :: readFile()
	{
		std::fstream db;
		db.open(loc);

		db.clear();
		db.seekg(0,std::ios::beg);

		std::string n;
		std::string line;
		double m,s;
		int c;

		while(getline(db,line)){
				std::istringstream is(line);

				is >> n >> m >> s >> c;

				SkillDatum sd(n,m,s,c);
				localTags.push_back(sd);
		}

		db.close();
	}

	SkillDatum SkillBase :: getTag(std::string n)
	{
			for(auto s: localTags)
				if(s.getName() == n)
					return s;

			localTags.push_back(SkillDatum(n));
			return localTags.back();
	}

	bool SkillBase :: setTag(SkillDatum sd)
	{
		for(auto &s: localTags)
			if(s.getName() == sd.getName()){
				s = sd;
				return true;
			}

		localTags.push_back(sd);
		return true;

	}

	void SkillBase :: writeFile()
	{

		std::ofstream db(loc);
		for(auto sd: localTags){
			db << sd.getName() << ' ' << sd.getMu() << ' ' << sd.getSigma() << ' ' << sd.getCount() << '\n';
		}

		db.close();

	}
}
namespace cute
{

	SkillHandle :: SkillHandle(std::vector<std::string> p1, std::vector<std::string> p2, SkillBase* sb)
		:base(sb)
	{

		int i =0;
		int team1mu = 0;
		int team2mu = 0;

		for(auto s: p1){
			team1.push_back(base->getTag(s));
			i++;
			team1mu += team1.back().getMu();


		}
		team1mu = team1mu / i;
		i =0;

		for(auto s: p2){
			team2.push_back(base->getTag(s));
			i++;
			team2mu += team2.back().getMu();


		}
		team2mu = team2mu / i;
		i =0;

		while(team1.size() < team2.size()){
			SkillDatum sd("meta",team1mu,team1mu/3,0);
			team1.push_back(sd);

		}

		while(team1.size() > team2.size()){
	SkillDatum sd("meta",team2mu,team2mu/3,0);
			team2.push_back(sd);

		}

	}

static std::vector< std::vector< double > > players;
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

	void SkillHandle :: run()
	{
		std::vector<SkillDatum*> iplayers;
		int skillId = 0;

		for(auto &i : team1){
			i.setTeam(1);
			i.setId(skillId++);
			i.iterateCount();
			iplayers.push_back(&i);
		}

		for(auto &i : team2){
			i.setTeam(2);
			i.setId(skillId++);
			i.iterateCount();

			i.iterateCount();
			iplayers.push_back(&i);

		}

		for(auto p : iplayers)
			addPlayer(p->getMu(),p->getSigma(),p->getTeam(),1,p->getId());



	std::vector< int > ranks;
	ranks.push_back(0);
	ranks.push_back(1);

	int teams = 2;


	rate(players,ranks,teams);

	for(auto i : iplayers){
		for(auto p : players){
			if(i->getId() == p[4])
			{
				i->setMu(p[0]);
				i->setSigma(p[1]);
			}
		}

	}

			


	}

	void SkillHandle :: setTags()
	{
		for(auto t : team1)
			base->setTag(t);

		for(auto t : team2)
			base->setTag(t);

	}




}
