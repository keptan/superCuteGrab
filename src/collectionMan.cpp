#include "collectionMan.h" 

namespace cute  {

CollectionMan :: CollectionMan (IdentityRank& i, std::vector< std::shared_ptr< Image>> c)
	: ident(i), collection(c), filtered(c), leftStreak(0), rightStreak(0) 
{
	freshImages();
}

std::vector< std::shared_ptr< Image>>
CollectionMan :: getImages (void)
{
	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return ident.getSkill(*a).mu > ident.getSkill(*b).mu;
				});

	return filtered;
}


void CollectionMan :: setImages (const std::vector<std::shared_ptr< Image>>> i)
{
	filtered = i;
}

void CollectionMan :: freshImages (void)
{
	ident.saveTags();
	std::random_device dev;
	std::mt19937 gen (dev());

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return ident.getSkill(*a).sigma > ident.getSkill(*b).sigma;
				});


	//get climb iters ready, the first ten percent of sigma images 
	const int tenPercent = (filtered.size() - 1) / 10;

	auto climbBottom = filtered.begin();
	auto climbTop = climbBottom + tenPercent; 

	while(climbTop >= filtered.end()) climbTop--;

	std::uniform_int_distribution<int> dist (0, tenPercent);
	leftImage  = *(climbBottom + dist(gen));
	rightImage = matchingImage(leftImage);

}
	
void CollectionMan :: leftVictory (void)
{

	ident.runImages(*leftImage, *rightImage, 1);

	leftStreak++;
	rightStreak = 0;

	if(leftStreak >= 10)
		return freshImages();

	rightImage = matchingImage(leftImage, leftStreak);

}

void CollectionMan :: rightVictory (void)
{

	ident.runImages(*leftImage, *rightImage, 2);

	leftStreak = 0;
	rightStreak++;

	if(rightStreak >= 10)
		return freshImages();

	leftImage= matchingImage( rightImage, rightStreak);

}

void CollectionMan :: tieVictory (void)
{
	ident.runImages(*leftImage, *rightImage, 3);
	freshImages();
}

void CollectionMan :: setRightImage ( std::shared_ptr< Image> i) 
{
	rightImage = i;
	leftImage = matchingImage(i);
}

void CollectionMan :: setLeftImage ( std::shared_ptr< Image> i) 
{
	leftImage = i;
	rightImage = matchingImage(i);
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
					return ident.getSkill(*a).mu > ident.getSkill(*b).mu;
				});

	const int tenPercent = (filtered.size() - 1) / 10;

	auto climbBottom = std::find( filtered.begin(), filtered.end(), i);
	auto climbTop = climbBottom + tenPercent;

	if(climbTop >= filtered.end()) climbTop = filtered.end() - 1;
	if(climbBottom >= filtered.end() - 20) climbBottom = (climbTop - 25 + winStreak);
	if(climbBottom >= filtered.end() - 5) climbBottom = (climbTop - 5);

	std::uniform_int_distribution<int> dist (0, std::abs(climbTop - climbBottom));


	while(true)
	{
		out = *(climbBottom + dist(gen));
		if(out != i)
			return out;
	}
}

}


