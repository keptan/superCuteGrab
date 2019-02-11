#include "collectionMan.h" 

namespace cute  {

CollectionMan :: CollectionMan (IdentityRank& i, PathRank& p, UserTags& t, std::vector< std::shared_ptr< Image>> c)
	: ident(i), path(p), tags(t), collection(c), filtered(c), leftStreak(0), rightStreak(0), runningFresh(false)
{
	//freshImages();
}

void CollectionMan :: setImages (const std::vector< std::shared_ptr<Image>> i)
{
	collection = i;
}
std::vector< std::shared_ptr< Image>>
CollectionMan :: getImages (void)
{
	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return ident.getSkill(*a).skill() > ident.getSkill(*b).skill();
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
					return ident.getSkill(*a).sigma > ident.getSkill(*b).sigma;
				});
	leftImage  = *(filtered.begin());
	rightImage = matchingELO(leftImage);

	runningFresh = ident.getSkill(*leftImage).sigma > 15 ? true : false; 
	std::cout << "running fresh: " << runningFresh << std::endl;
}
	
void CollectionMan :: leftVictory (void)
{
	runImages(1);
	rightStreak = 0;
	leftStreak++;

	const bool leftUncertain  = ident.getSkill(*leftImage).sigma  > 9; 
	const bool rightUncertain = ident.getSkill(*rightImage).sigma > 9; 

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

	const bool leftUncertain  = ident.getSkill(*leftImage).sigma  > 9; 
	const bool rightUncertain = ident.getSkill(*rightImage).sigma > 9; 

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

	const bool leftUncertain  = ident.getSkill(*leftImage).sigma  > 9; 
	const bool rightUncertain = ident.getSkill(*rightImage).sigma > 9; 


	if(runningFresh && leftUncertain  && leftStreak < 10)
	{
		leftStreak++;
		rightImage = matchingELO( leftImage, 0);
		return; 
	}

	freshImages();
}

void CollectionMan :: setRightImage ( std::shared_ptr< Image> i) 
{
	rightImage = i;
	leftImage = matchingELO(i);
}

void CollectionMan :: setLeftImage ( std::shared_ptr< Image> i) 
{
	leftImage = i;
	rightImage = matchingELO(i);
}

SkillDatum CollectionMan :: getSkill (std::shared_ptr< Image> i)
{
	return ident.getSkill(*i);
}

std::shared_ptr< Image> CollectionMan :: getRightImage (void)
{
	return rightImage;
}

std::shared_ptr< Image> CollectionMan :: getLeftImage (void)
{
	return leftImage; 
}


std::shared_ptr< Image> CollectionMan :: matchingImage (std::shared_ptr< Image> i, int winStreak)
{
	std::shared_ptr< Image> out = nullptr;

	std::random_device dev;
	std::mt19937 gen (dev());

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return ident.getSkill(*a).skill() < ident.getSkill(*b).skill();
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

std::shared_ptr< Image> CollectionMan :: matchingELO (std::shared_ptr< Image> i, int streak )
{
	std::shared_ptr< Image> out = nullptr; 
	std::random_device dev; 
	std::mt19937 gen (dev()); 

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return ident.getSkill(*a).skill() < ident.getSkill(*b).skill();
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
	std::cout << ident.getSkill(**(climbBottom)).skill() << " and ";
	std::cout << ident.getSkill(**(climbTop)).skill() << "range of: " << (climbTop - climbBottom) << std::endl;
	std::cout << "left image, mu: " << ident.getSkill(*(leftImage)).mu << " sigma: " << ident.getSkill(*(leftImage)).sigma << std::endl;



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

	ident.runImages(*leftImage, *rightImage, winner);
	path.runImages(*leftImage, *rightImage, winner);
	tags.runImages(*leftImage, *rightImage, winner);

}

void CollectionMan :: saveTags (void)
{
	ident.saveTags();
	path.saveTags();
	tags.saveTags();
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
		bool found = true;
		auto itags = tags.tags.retrieveData( i->pData.hash);

		for(const auto t : tokens)
		{
			if(!found) break; 
			if(!itags.contains(t)) found = false;
		}
		for(const auto t : anti_tokens)
		{
			if(!found) break;
			if(itags.contains(t)) found = false;
		}

		if(found) filtered.push_back(i);
	}
}




				}

