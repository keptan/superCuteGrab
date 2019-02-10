#pragma once 
#include "tagDB.h"
#include "scoreDB.h" 
#include "image.h" 
#include "tagDB.h"

namespace cute		{

class CompMan 
{
	public:
	virtual void runImages (const Image&, const Image&, int) = 0;
	virtual void saveTags (void) = 0;
	virtual ~CompMan (void);
};

class IdentityRank : public CompMan 
{

	ScoreDB scores; 

	public:
	IdentityRank (void);
	void runImages (const Image& , const Image& , int );
	void saveTags (void);

	SkillDatum getSkill (const Image&);

};

class PathRank : public CompMan 
{
	ScoreDB scores;

	public:
	PathRank (void);
	void runImages (const Image&, const Image&, int );
	void saveTags (void);

	SkillDatum getSkill (const Image&);
};

class UserTags : public CompMan
{
	public:

	TagDB tags;
	ScoreDB scores;

	UserTags (void);
	void runImages (const Image&, const Image&, int);
	void saveTags (void);

	TagSet getTags (const Image&);

	void insert (const Image&, const TagSet&);
	
};

					}
