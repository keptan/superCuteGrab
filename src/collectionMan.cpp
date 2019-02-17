#include "collectionMan.h" 

namespace cute  {

CollectionMan :: CollectionMan (IdentityRank& id, PathRank& path, ComTags& u, ComTags& b, ComTags& a, ComTags& c,
				   std::vector< SharedImage> i)
			  : collection(i), identityRanker(id), pathRanker(path), userTags(u), booruTags(b), artistTags(a), 
			    charTags(c), allTags({ &a, &c, &u, &b})
{
	//freshImages();
}

void CollectionMan :: setImages (const std::vector< SharedImage> i)
{
	collection = i;
}

std::vector< SharedImage>
CollectionMan :: getImages (void)
{
	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return identityRanker.getSkill(*a).skill() > identityRanker.getSkill(*b).skill();
				});

	return filtered;
}




void CollectionMan :: freshImages (void)
{
	rightStreak = 0;
	leftStreak = 0;

	saveTags();

	std::random_device dev;
	std::mt19937 gen (dev());

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return identityRanker.getSkill(*a).sigma > identityRanker.getSkill(*b).sigma;
				});
	leftImage  = *(filtered.begin());
	rightImage = matchingELO(leftImage);

	//randomize side here please!
	runningFresh = identityRanker.getSkill(*leftImage).sigma > 15 ? true : false; 
	std::cout << "running fresh: " << runningFresh << std::endl;
}
	
void CollectionMan :: leftVictory (void)
{
	runImages(1);
	rightStreak = 0;
	leftStreak++;

	const bool leftUncertain  = identityRanker.getSkill(*leftImage).sigma  > 9; 
	const bool rightUncertain = identityRanker.getSkill(*rightImage).sigma > 9; 

	if(rightStreak > 10 || leftStreak > 10)
		return freshImages();

	if(leftUncertain && runningFresh) 
	{
		rightImage = matchingELO(leftImage, 0); 
		return;
	}

	rightImage = matchingELO( leftImage, leftStreak);
}

void CollectionMan :: rightVictory (void)
{
	runImages(2);
	rightStreak++;

	const bool leftUncertain  = identityRanker.getSkill(*leftImage).sigma  > 9; 
	const bool rightUncertain = identityRanker.getSkill(*rightImage).sigma > 9; 

	if(rightStreak > 10 || leftStreak > 10)
	{
		leftStreak = 0;
		return freshImages();
	}

	if(leftUncertain && runningFresh) 
	{
		leftStreak++;
		rightImage = matchingELO(leftImage, 0); 
		return; 
	}

	leftStreak = 0;
	leftImage = matchingELO( rightImage, rightStreak);
}

void CollectionMan :: tieVictory (void)
{
	runImages(3);

	const bool leftUncertain  = identityRanker.getSkill(*leftImage).sigma  > 9; 
	const bool rightUncertain = identityRanker.getSkill(*rightImage).sigma > 9; 


	if(runningFresh && leftUncertain  && leftStreak < 10)
	{
		leftStreak++;
		rightImage = matchingELO( leftImage, 0);
		return; 
	}

	freshImages();
}

void CollectionMan :: setRightImage ( SharedImage i) 
{
	rightImage = i;
	leftImage = matchingELO(i);
}

void CollectionMan :: setLeftImage ( SharedImage i) 
{
	leftImage = i;
	rightImage = matchingELO(i);
}


SharedImage CollectionMan :: getRightImage (void)
{
	return rightImage;
}

SharedImage CollectionMan :: getLeftImage (void)
{
	return leftImage; 
}


SharedImage CollectionMan :: matchingImage (SharedImage i, int winStreak)
{
	std::cout << "finding matching image" << std::endl;
	SharedImage out = nullptr;

	std::random_device dev;
	std::mt19937 gen (dev());

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return identityRanker.getSkill(*a).skill() < identityRanker.getSkill(*b).skill();
				});

	const int tenPercent = (filtered.size() - 1) / 10;

	auto climbBottom = std::find( filtered.begin(), filtered.end(), i);
	auto climbTop = climbBottom + tenPercent + (winStreak * (tenPercent / 2));

	if(climbBottom - tenPercent > filtered.begin()) climbBottom = climbBottom - (tenPercent / 2);
	if(climbTop >= filtered.end()) climbTop = filtered.end() - 1;
	if(climbBottom >= filtered.end() - tenPercent) climbBottom = (climbTop - 25 + winStreak);
	if(climbBottom >= filtered.end() - 5) climbBottom = (climbTop - 5);


	std::uniform_int_distribution<int> dist (0, std::abs(climbTop - climbBottom));


	while(true)
	{
		out = *(climbBottom + dist(gen));
		if(out != i)
			return out;
	}
}

SharedImage CollectionMan :: matchingELO (SharedImage i, int streak )
{
	SharedImage out = nullptr; 
	std::random_device dev; 
	std::mt19937 gen (dev()); 

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return identityRanker.getSkill(*a).skill() < identityRanker.getSkill(*b).skill();
				});


	const int twoPercent = std::max<int>( ((filtered.size() - 1  ) / 50), 5);

	auto climbBottom = std::find( filtered.begin(), filtered.end(), i) - twoPercent;
	auto climbTop = climbBottom + (twoPercent * 2);


	if(!runningFresh) climbBottom += (twoPercent * (streak / 3 + 1));
	if(!runningFresh) climbTop += (twoPercent * (streak / 2));
	if(climbTop >= filtered.end()) climbTop = filtered.end() - 1;
	if(climbBottom < filtered.begin()) climbBottom = filtered.begin(); 

	if(climbBottom >= filtered.end() - 15) climbBottom = filtered.end() - 15;


	std::cout << "matching image between ";
	std::cout << identityRanker.getSkill(**(climbBottom)).skill() << " and ";
	std::cout << identityRanker.getSkill(**(climbTop)).skill() << "range of: " << (climbTop - climbBottom) << std::endl;
	std::cout << "left image, mu: " << identityRanker.getSkill(*(leftImage)).mu << " sigma: " << identityRanker.getSkill(*(leftImage)).sigma << std::endl;



	std::uniform_int_distribution<int> dist (0, std::abs(climbTop - climbBottom));

	while(true)
	{
		out = *(climbBottom + dist(gen));
		if(out != i)
			return out;
	}
}

void CollectionMan :: runImages (const int winner)
{

	identityRanker.runImages(*leftImage, *rightImage, winner);
	pathRanker.runImages(*leftImage, *rightImage, winner);


	for(auto t : allTags)
	{
		t->runImages(*leftImage, *rightImage, winner);
	}

}

void CollectionMan :: saveTags (void)
{
	identityRanker.saveTags();
	pathRanker.saveTags();

	for(auto t : allTags)
	{
		t->saveTags();
	}
}

void CollectionMan :: filter (void)
{
	filtered = collection;
}

void CollectionMan :: filter (const std::string& str)
{
	std::vector<std::string> tokens;
	std::vector<std::string> anti_tokens;

	const auto tSort = [&](const auto s)
	{
		if(s.size() <= 2) return; 
		if(s[0] == '!') 
		{
			anti_tokens.push_back(s.substr(1));
			return;
		}
		tokens.push_back(s);
	};

	std::string acc;
	for(const auto c: str)
	{
		if(c == ' ') continue;
		if(c == ',') 
		{
			tSort(acc);
			acc = "";
			continue;
		}

		acc += c;
	}

	tSort(acc);
	filtered.clear();

	for(const auto i : collection)
	{
		bool found = false;
		auto itags = [&](void)
		{
			TagSet acc;

			for(auto db : allTags)
			{
				acc += db->tags.retrieveData( i->pData.hash);
			}

			return acc;
		}();

		for(const auto t : tokens)
		{
			found = itags.contains(t);
		}
		for(const auto t : anti_tokens)
		{
			found = !itags.contains(t);
		}

		if(found) filtered.push_back(i);
	}
}


TagSet CollectionMan :: retrieveTags (void)
{
	TagSet acc;

	for(auto db : allTags)
	{
		acc += db->tags.retrieveData();
	}

	return acc;
}

TagSet CollectionMan :: retrieveTags (SharedImage i)
{
	TagSet acc;

	for(auto db : allTags)
	{
		acc += db->tags.retrieveData( i->pData.hash);
	}

	return acc;
}

TagSet CollectionMan :: retrieveTags (const std::vector<SharedImage>& images)
{
	TagSet acc;

	for(auto db : allTags)
	{
		for(const auto i : images) acc += db->tags.retrieveData( i->pData.hash);
	}

	return acc;
}

std::vector< std::tuple<Tag, SkillDatum>> CollectionMan :: tagsWithScores (void)
{
	std::vector< std::tuple<Tag, SkillDatum>> acc;

	for(auto db : allTags)
	{
		const auto tags = db->tags.retrieveData();
		for(const auto t : tags)
		{
			acc.push_back( std::make_tuple( t, db->scores.retrieveData(t)));
		}
	}

	return acc;
}

std::vector< std::tuple<Tag, SkillDatum>> CollectionMan :: tagsWithScores (SharedImage i)
{
	std::vector< std::tuple<Tag, SkillDatum>> acc;

	for(auto db : allTags)
	{
		const auto tags = db->tags.retrieveData( i->pData.hash);
		for(const auto t : tags)
		{
			acc.push_back( std::make_tuple( t, db->scores.retrieveData(t)));
		}
	}

	return acc;
}

std::vector< std::tuple<Tag, SkillDatum>> CollectionMan :: tagsWithScores (const std::vector<SharedImage>& images)
{
	std::map< Tag, SkillDatum> acc;
	std::vector< std::tuple<Tag, SkillDatum>> out;

	for(auto db : allTags)
	{
		for(const auto i : images)
		{
			const auto tags = db->tags.retrieveData( i->pData.hash);
			for(const auto t : tags)
			{
				acc.insert( std::make_pair( t, db->scores.retrieveData(t)));
			}
		}
	}


	for(const auto p : acc)
	{
		out.push_back( std::make_tuple( p.first, p.second));
	}


	return out;
}




				}

