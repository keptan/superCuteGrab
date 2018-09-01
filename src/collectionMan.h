#pragma once 
#include "image.h" 
#include "tagSet.h" 
#include "compMan.h" 
#include <memory>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

namespace cute		{

class CollectionMan 
{
	IdentityRank ident; 
	//other rankers here 
	
	std::vector< std::shared_ptr< Image>> collection; 
	std::vector< std::shared_ptr< Image>> filtered; 

	std::shared_ptr< Image> leftImage; 
	std::shared_ptr< Image> rightImage; 

	std::set< std::shared_ptr< Image>> history;

	int leftStreak, rightStreak;

	public:
	CollectionMan (IdentityRank&, std::vector< std::shared_ptr< Image>> c);

	void freshImages (void);
	std::shared_ptr< Image> getRightImage (void); 
	std::shared_ptr< Image> getLeftImage  (void);
	std::vector< std::shared_ptr< Image>> getImages (void); 
	void  setImages (const std::vector<std::shared_ptr< Image>>);

	void setRightImage ( std::shared_ptr< Image>);
	void setLeftImage  ( std::shared_ptr< Image>);

	SkillDatum getSkill (std::shared_ptr< Image>);

	void leftVictory  (void);
	void rightVictory (void);
	void tieVictory   (void);

	//pin image, ect 

	private:

	std::shared_ptr< Image> matchingImage ( std::shared_ptr< Image>, int winStreak = 0);


};

		}


