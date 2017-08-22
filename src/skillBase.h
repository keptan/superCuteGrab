#ifndef SKILLBASE_H
#define SKILLBASE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <memory>

namespace cute
{
	class SkillDatum
	{
		private :
			std::string tagName;
			int tagMu;
			int tagSigma;
			int tagCount;

		public :
			SkillDatum(std::string n,int m , int s , int c );
			
			int getMu();
			int getSigma();
			int getCount();
			std::string getName();

			void setMu(int m);
			void setSigma(int s);
			void iterateCount();

	};

	class SkillBase
	{
		private :
			std::fstream db;
			std::list<SkillDatum> localTags;

		public :
			SkillBase(std::string l);

			bool hasTag(std::string n);
			SkillDatum getTag(std::string);
			bool setTag(SkillDatum sd);

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
			SkillHandle(std::vector<std::string> p1, p2,SkillBase* sb);
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
