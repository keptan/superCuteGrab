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

	//all the images we can accecss, and a filtered version that we look for potential comparisons in 
	//passing around shared pts same cost as passing around iterators but 
	//without invalidation
	std::vector< SharedImage> collection; 
	std::vector< SharedImage> filtered;

	//current images being compared
	SharedImage leftImage; 
	SharedImage rightImage; 

	//will be a log of images we ranked already, so we can not compare them again 
	std::set< SharedImage> history;

	int leftStreak, rightStreak;
	bool runningFresh;

	public:
	CollectionMan (IdentityRank&, PathRank&, UserTags&, std::vector< SharedImage> = {});

	IdentityRank& identityRanker;
	PathRank&	  pathRanker;
	UserTags&	  tags; 
	/*
	UserTags&	  artist;
	UserTags&	  character;
	*/

	void freshImages (void); //set left and right to clean images
	SharedImage getRightImage (void); 
	SharedImage getLeftImage  (void);

	std::vector< SharedImage> getImages (void); //get the collection
	void  setImages (const std::vector<SharedImage>); //set the collection

	void setRightImage ( SharedImage); //override left or right image
	void setLeftImage  ( SharedImage);

	//run image scores
	void leftVictory  (void);
	void rightVictory (void);
	void tieVictory   (void);

	void filter (void);
	void filter (const std::string&);


	//pin image, ect 

	private:

	SharedImage matchingImage ( SharedImage, int winStreak = 0);
	SharedImage matchingELO   ( SharedImage, int streak = 0);

	void runImages (const int);
	void saveTags (void);


};

		}

