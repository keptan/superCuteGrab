#pragma once 
#include "tagDB.h"
#include "scoreDB.h" 
#include "image.h" 

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

	public:
	ScoreDB scores; 

	IdentityRank (void);
	void runImages (const Image& , const Image& , int );
	void saveTags (void);

	SkillDatum getSkill (const Image&);

};

}