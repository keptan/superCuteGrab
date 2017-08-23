#ifndef SKILLBASE_H
#define SKILLBASE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include "trueskill/trueskill.h"
#include <memory>

namespace cute
{
	class SkillDatum :  public  Player
	{
		private :
			std::string tagName;
			int count;

		public :
			SkillDatum(std::string n,double m = 100, double s = 33, double c = 0);
			
			int getMu();
			int getSigma();
			int getCount();
			std::string getName();

			void setMu(double m);
			void setSigma(double s);
			void iterateCount();

	};

	class SkillBase
	{
		private :
			std::string loc;
			std::list<SkillDatum> localTags;

		public :
			SkillBase(std::string l);

			bool hasTag(std::string n);
			SkillDatum getTag(std::string);
			bool setTag(SkillDatum sd);
			void writeFile();

			bool readFile();
				
		};
		
	class SkillHandle
	{
		private:
			std::vector<SkillDatum> team1;
			std::vector<SkillDatum> team2;
			SkillBase* base;


			void prepareTeams();

		public:
			SkillHandle(std::vector<std::string> p1, std::vector<std::string> p2,SkillBase* sb);
			void run();
			void setTags();
	};
}
#endif

//example skillHandle interface
//skill datum shared by metaData.h???
//
//image has tags
//we want to convert those tags into skilldatum
//and then take collections of skill datum as vectors
//skill handle can take metadata.cpp getTags()
//as team 1 and team 2
//and then uses trueskill library  after using skillBase to get the mu and sigmas for each tag 
//using getTag() 
//so really metaData just needs to be strings not datum
//even if it would be nice
//SkillHandle.setTeams(player1.getTags,player2.getTags);
//SkillHandle.run();
//SkillHandle.setTags(sb);

//make skilldatum inhereit player class or soemyhing
