#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

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

	SkillBase :: SkillBase(std::string l)
	{
		db.open(l);
	}

	bool SkillBase :: hasTag(std::string n)
	{
		db.clear();
		db.seekg(0,std::ios::beg);

		std::istringstream is;
		for (auto s : localTags)
			if(s.getName() == n)
				return true;

		std::string line;
		while(getline(db,line)){
			if(line.find(n) != std::string::npos)
				return true;
		}

		return false;
	}

	bool SkillBase :: hasTagLocal(std::string n)
	{	
		for (auto s : localTags)
			if(s.getName() == n)
				return true;

		return false;
	}

	SkillDatum* SkillBase :: getTag(std::string n)
	{
		db.clear();
		db.seekg(0,std::ios::beg);

		std::string line;
		std::string name;
		int mu, sigma, count;

		while(getline(db,line)){
				if(line.find(n) != std::string::npos){

					std::istringstream is(line);


					is >> name >> mu >> sigma >> count;

					SkillDatum sd(name,mu,sigma,count);
					localTags.push_back(sd);
					return &localTags.back();
				}

			}
			return NULL;
	}
}









		

		
	
	//return a SkilLDatum instead and then can assign locally
