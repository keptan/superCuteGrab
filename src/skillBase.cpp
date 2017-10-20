#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cassert>

#include <algorithm>  

#include <experimental/random>
#include "cskill/trueskill.h"
#include "skillBase.h"
#include "metaData.h"

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

	bool SkillDatum :: isDup()
	{
		return dup;
	}

	void SkillDatum :: setDup(bool d)
	{
		dup = d;
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
		localTags.clear();
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

	SkillDatum SkillBase :: getTag(MetaData m)
	{
		std::string tagHash = m.getTagMd5();
		if(tagHash.size() == 0){
		std::ostringstream n;
		n << "MD5:"<<m.getHash();
		m = n.str();
		}



		for(auto s: localTags)
				if(s.getName() == tagHash)
					return s;

			localTags.push_back(SkillDatum(tagHash));
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
localTags.sort([]( auto & a,  auto & b) { return ((a.mu * a.mu + a.mu) /a.getSigma()  > (b.mu * b.mu * b.mu )/b.getSigma()); });
//localTags.sort([]( auto & a,  auto & b) { return a.mu > b.mu; });
		

		std::ofstream db(loc);
		for(auto sd: localTags){
			db << sd.getName() << ' ' << sd.getMu() << ' ' << sd.getSigma() << ' ' << sd.getCount() << '\n';
		}

		db.close();

	}
}
namespace cute
{

	SkillHandle :: SkillHandle(MetaData m1, MetaData m2, SkillBase* sb,int t)
		:base(sb),tie(t)
	{

		int i =0;
		int x =0;
		int team1mu = 0;
		int team2mu = 0;
		int team3mu = 0;


		std::vector<std::string> p1 = m1.getTags();
		std::vector<std::string> p2 = m2.getTags();

		std::vector<std::string> p3;

		for(auto s: p1){
			std::string c = "MD5";
			if (std::find(p2.begin(), p2.end(),s) != p2.end()){
				x++;
				team3.push_back(base->getTag(s));
				team3.back().setDup(true);
				team3mu += team3.back().getMu();

				}
			else{
				

			if(c == s.substr(0,c.length())){
				name1 = base->getTag(s);
			}
			else{

			team1.push_back(base->getTag(s));
			i++;
			team1mu += team1.back().getMu();
			}
			}

		}
		if(team1.size() > 0 )
		{
		team1mu = team1mu / i;
		if(team3.size() != 0)
		team3mu = team3mu / x;
		i =0;

		for(auto s: p2){

			std::string c = "MD5";
			if (std::find(p1.begin(), p1.end(),s) == p1.end()){

			if(c == s.substr(0,c.length())){
				name2 = base->getTag(s);
			}
			else{
			team2.push_back(base->getTag(s));
			i++;
			team2mu += team2.back().getMu();
			}
		}
		}
		if(team2.size() > 0)
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

		if(team3.size() > 0){
		while(team1.size() > team3.size()){
	SkillDatum sd("meta",team3mu,team3mu/3,0);
			team3.push_back(sd);
		}


		}
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
		players.clear();
		std::vector<SkillDatum*> iplayers;
		int skillId = 0;

		std::cout<<"team1"<<team1.size()<<'\n';
		std::cout<<"team2"<<team2.size()<<'\n';

		std::cout<<"team3"<<team3.size()<<'\n';


		if(team1.size() < 1 || team2.size() < 1){
			std::cout<<"exiting because of 0 team sizes";
			return;
		}

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

			iplayers.push_back(&i);


		}

	for(auto &i : team3){
			i.setTeam(3);
			i.setId(skillId++);
			i.iterateCount();





			iplayers.push_back(&i);

		}

		for(auto p : iplayers)
			addPlayer(p->getMu(),p->getSigma(),p->getTeam(),1,p->getId());


	std::vector< int > ranks;
	int teams;

	if(team3.size() != 0){
	if(tie < 0)
	{
	ranks.push_back(0);
	ranks.push_back(0);
	ranks.push_back(0);
	}else{
	ranks.push_back(0);
	ranks.push_back(2);
	ranks.push_back(1);
	}

	 teams = 3;
	}
	else{
	if(tie < 0)
	{
	ranks.push_back(0);
	ranks.push_back(0);
	}else{
	ranks.push_back(0);
	ranks.push_back(1);
	}

	 teams = 2;
	}


	assert(team1.size() > 0 && team2.size() > 0);
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
	
	iplayers.clear();
	ranks.clear();
	players.clear();

	teams = 2;

	name1.setTeam(1);
	name1.setId(skillId++);
	name1.iterateCount();

	iplayers.push_back(&name1);

	name2.setTeam(2);
	name2.setId(skillId++);
	name2.iterateCount();

	iplayers.push_back(&name2);

	for(auto p : iplayers)
			addPlayer(p->getMu(),p->getSigma(),p->getTeam(),1,p->getId());

	if(tie < 0)
	{
		ranks.push_back(0);
		ranks.push_back(0);
	}
	else{
		ranks.push_back(0);
		ranks.push_back(1);
	}

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
		for(auto t : team3)
			base->setTag(t);

		base->setTag(name1);
		base->setTag(name2);

	}




}

namespace cute
{

FileDatum :: FileDatum(std::string n,std::string l)
	:fileName(n),fileLoc(l){}

FileDatum :: FileDatum(MetaData m)
{
	fileName = m.getTagMd5();
	fileLoc = m.filePath();
}

void FileDatum :: setName(std::string n)
{
	fileName = n;
}

void FileDatum :: setLoc(std::string l)
{
	fileLoc = l;
}

std::string FileDatum :: getLoc()
{
	return fileLoc;
}

std::string FileDatum :: getName()
{
	return fileName;
}




FileBase :: FileBase(std::string l)
	: loc(l)
	{
		readFile();
	}

bool FileBase :: readFile()
{
		std::fstream db;
		localTags.clear();
		db.open(loc);

		db.clear();
		db.seekg(0,std::ios::beg);

		std::string n;
		std::string l;
		std::string line;

		while(getline(db,line)){
			if(line.size() > 2){
				n = line.substr(0,line.find(' '));
				l = line.substr(line.find(' ')+1,line.size());

				FileDatum fd(n,l);
				localTags.push_back(fd);
			}
		}

		db.close();
}
	
void FileBase :: writeFile()
	{
		

		std::ofstream db(loc);
		for(auto fd: localTags){
			db << fd.getName() <<' '<< fd.getLoc() << '\n';
		}

		db.close();

	}

bool FileBase :: setTag(FileDatum fd)
	{
		for(auto &s: localTags)
			if(s.getName() == fd.getName()){
				s = fd;
				return true;
			}

		localTags.push_back(fd);
		return true;

	}

	FileDatum  FileBase :: getTag(MetaData m)
	{
		std::ostringstream n;

		for(auto s: localTags)
				if(s.getName() == n.str())
					return s;
	}

	FileDatum FileBase :: getRandom()
{

		int r = std::experimental::randint(0,(int)localTags.size() -1);

		return localTags[r];

}

	
	void FileBase :: addTag(MetaData m)
{
	FileDatum fd(m);

	for(auto s: localTags){
		if(s.getName() == fd.getName()){
			s = fd;
			return;
		}
	}

	localTags.push_back(fd);
}

void FileBase :: clear()
{
	localTags.clear();
}



}
