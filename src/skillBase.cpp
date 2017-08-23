#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>


#include "trueskill/trueskill.h"
#include "skillBase.h"

namespace cute
{
	SkillDatum :: SkillDatum(std::string n,double m, double s, double c )
		:Player(m,s),tagName(n),count(c)
	{}
	
	std::string SkillDatum :: getName()
	{
		return tagName;
	}

	int SkillDatum :: getMu()
	{
		return mu;
	}

	int SkillDatum :: getSigma()
	{
		return sigma;
	}

	int SkillDatum :: getCount()
	{
		return count;
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
		for(auto sd: localTags)
			db << sd.getName() << ' ' << sd.getMu() << ' ' << sd.getSigma() << ' ' << sd.getCount() << '\n';

		db.close();

	}
}
namespace cute
{

	SkillHandle :: SkillHandle(std::vector<std::string> p1, std::vector<std::string> p2, SkillBase* sb)
		:base(sb)
	{

		int i =0;
		int team1mu;
		int team2mu;

		for(auto s: p1){
			team1.push_back(base->getTag(s));
			i++;
			team1mu + team1.back().getMu();

		}
		team1mu /= i;
		i =0;

		for(auto s: p2){
			team2.push_back(base->getTag(s));
			i++;
			team2mu + team2.back().getMu();

		}
		team2mu /= i;
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

	void SkillHandle :: run()
	{
		std::vector<Player*> players;
		for(auto &i : team1){
			i.rank = 1;
			i.iterateCount();
			players.push_back(*i);
		}

		for(auto &i : team2){
			i.rank = 2;
			i.iterateCount();

			players.push_back(*i);
		}

	TrueSkill ts;
	ts.adjust_players(players);

	}

	void SkillHandle :: setTags()
	{
		for(auto t : team1)
			base->setTag(t);

		for(auto t : team2)
			base->setTag(t);

	}




}
