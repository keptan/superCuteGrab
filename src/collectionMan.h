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
	IdentityRank& ident; 
	PathRank& path;

	//other rankers here 
	
	//all the images we can accecss, and a filtered version that we look for potential comparisons in 
	std::vector< std::shared_ptr< Image>> collection; 

	//current images being compared
	std::shared_ptr< Image> leftImage; 
	std::shared_ptr< Image> rightImage; 

	//will be a log of images we ranked already, so we can not compare them again 
	std::set< std::shared_ptr< Image>> history;

	int leftStreak, rightStreak;
	bool runningFresh;

	public:
	CollectionMan (IdentityRank&, PathRank&, UserTags&, std::vector< std::shared_ptr< Image>>);

	UserTags& tags;
	void freshImages (void); //set left and right to clean images
	std::shared_ptr< Image> getRightImage (void); 
	std::shared_ptr< Image> getLeftImage  (void);
	std::vector< std::shared_ptr< Image>> getImages (void); //get the collection
	void  setImages (const std::vector<std::shared_ptr< Image>>); //set the collection

	void setRightImage ( std::shared_ptr< Image>); //override left or right image
	void setLeftImage  ( std::shared_ptr< Image>);

	SkillDatum getSkill (std::shared_ptr< Image>); //get the skill of an image

	//run image scores
	void leftVictory  (void);
	void rightVictory (void);
	void tieVictory   (void);


	//pin image, ect 

	private:

	std::shared_ptr< Image> matchingImage ( std::shared_ptr< Image>, int winStreak = 0);
	std::shared_ptr< Image> matchingELO   ( std::shared_ptr< Image>, int streak = 0);

	void runImages (const int);
	void saveTags (void);


};

		}

