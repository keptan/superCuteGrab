#include "compMan.h" 
#include "cskill_instance.h"
#include <iostream>

namespace cute {

CompMan :: ~CompMan (void) {}

	
IdentityRank :: IdentityRank (void) 
	: scores("data/idScores.csv", 0)
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

PathRank :: PathRank (void)
	: scores("data/pathScores.csv")
{}

void PathRank :: runImages (const Image& p1, const Image& p2, int winner)
{
	TagSet player1, player2;
	player1 += Tag(p1.location.parent_path().string());
	player2 += Tag(p2.location.parent_path().string());

	CSkillInstance cskill (scores);
	cskill.generateTeams(player1, player2);
	cskill.runScores(winner);
}

void PathRank :: saveTags (void)
{
	scores.writeCSV();
}

SkillDatum PathRank :: getSkill (const Image& i)
{
	return scores.retrieveData( i.location.parent_path().string());
}

ComTags :: ComTags (const std::string& t, const std::string& s)
	: tags(t), scores(s)
{}

void ComTags :: runImages (const Image& p1, const Image& p2, int winner)
{
	TagSet player1 = tags.retrieveData( p1.pData.hash);
	TagSet player2 = tags.retrieveData( p2.pData.hash);

	CSkillInstance cskill (scores);
	cskill.generateTeams(player1, player2);
	cskill.runScores(winner);
}

void ComTags :: saveTags (void)
{
	scores.writeCSV();
}

TagSet ComTags :: getTags (const Image& i)
{
	return tags.retrieveData( i.pData.hash);
} 

void ComTags :: insert (const Image& i, const TagSet& s)
{
	tags.insertTags(i.pData.hash, s);
	tags.writeCSV();
}

			}
