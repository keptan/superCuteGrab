#pragma once 
#include "trueskill.h" 
#include "image.h" 
#include "tagSet.h" 
#include "scoreDB.h"

namespace cute		{

	class CSkillInstance 
	{
		ScoreDB& scores; 
		TagSet team1, team2,team3;

		std::vector<std::vector<double>> players; 
		std::map<int, Tag> idTags;

		public:
		CSkillInstance (ScoreDB&);
		void generateTeams (const TagSet& player1, const TagSet& player2);
		void runScores (int);

		private:

		void eatVector (void);
		void addPlayer(double, double, double, double);
		void addPlayer (const SkillDatum& t, int team, int id);

	};

					}



		
