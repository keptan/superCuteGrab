#include "compMan.h" 
#include "cskill_instance.h"

namespace cute {

CompMan :: ~CompMan (void) {}

	
IdentityRank :: IdentityRank (void) 
	: scores("data/idScores.csv")
{}

void IdentityRank :: runImages (const Image& p1, const Image& p2, int winner)
{
	TagSet player1, player2; 
	player1 += Tag(p1.pData.hash);
	player2 += Tag(p2.pData.hash);

	CSkillInstance cskill (scores);
	cskill.generateTeams(player1, player2);
	cskill.runScores(winner);
}

void IdentityRank :: saveTags (void)
{
	scores.writeCSV();
}

SkillDatum IdentityRank :: getSkill (const Image& i)
{
	return scores.retrieveData( i.pData.hash);
}




}
