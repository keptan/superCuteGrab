#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "skillBase.h"

namespace cute
{
	SkillDatum :: SkillDatum(std::string n,int m = 100, int s = 33, int c = 0)
		:tagName(n),tagMu(m),tagSigma(s),tagCount(c)
	{}
	
	std::string SkillDatum :: getName()
	{
		return tagName;
	}

	int SkillDatum :: getMu()
	{
		return tagMu;
	}

	int SkillDatum :: getSigma()
	{
		return tagSigma;
	}

	int SkillDatum :: getCount()
	{
		return tagCount;
	}

	void SkillDatum :: setMu(int m)
	{
		tagMu = m;
	}

	void SkillDatum :: setSigma(int s)
	{
		tagSigma = s;
	}

	void SkillDatum :: iterateCount()
	{
		tagCount += 1;
	}

}
namespace cute
{
	SkillBase :: SkillBase(std::string l)
	{
		db.open(l);
		readFile();
	}

	bool SkillBase :: hasTag(std::string n)
	{
		db.clear();
		db.seekg(0,std::ios::beg);

		std::istringstream is;
		for(auto s: localTags)
			if(s.getName() == n)
				return true;

		return false;
	}
	
	bool SkillBase :: readFile()
	{
		db.clear();
		db.seekg(0,std::ios::beg);

		std::string n;
		std::string line;
		int m,s,c;

		while(getline(db,line)){
				std::istringstream is(line);

				is >> n >> m >> s >> c;

				SkillDatum sd(n,m,s,c);
				localTags.push_back(sd);
		}
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
				std::cout<<"match found\n";
				s = sd;
				return true;
			}

		return false;
	}
}
namespace cute
{

	SkillHandle :: SkillHandle(std::vector<std::string> p1, p2, SkillBase* sb)
		:base(sb)
	{

		int i =0;
		int team1mu;
		int team2mu;

		for(auto s: p1){
			team1.push_back(base->getTag(s));
			i++;
			team1mu + team1.back.getMu();

		}
		team1mu /= i;
		i =0;

		for(auto s: p2){
			team2.push_back(base->getTag(s));
			i++;
			team2mu + team2.back.getMu();

		}
		team2mu /= i;
		i =0;

		while(team1.size() < team2.size())
			team1.push_back(new SkillDatum("meta",team1mu,team1mu/3,0));

		while(team1.size() > team2.size())
			team2.push_back(new SkillDatum("meta",team2mu,team2mu/3,0));

	}




}
