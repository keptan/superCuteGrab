#ifndef SKILLBASE_H
#define SKILLBASE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

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
			std::vector<SkillDatum> localTags;

		public :
			SkillBase(std::string l);

			bool hasTag(std::string n);
			bool hasTagLocal(std::string n);

			SkillDatum* getTag(std::string);


			

				
		};
			//format for storing this stuff;
			//data stored: number of times rated, trueskill score and mu, winrate

			//search vector and then search file when requested

}
#endif
