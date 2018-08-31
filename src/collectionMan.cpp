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

void CollectionMan :: freshImages (void)
{
	ident.saveTags();
	std::random_device dev;
	std::mt19937 gen (dev());

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return ident.getSkill(*a).sigma < ident.getSkill(*b).sigma;
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
	std::random_device dev;
	std::mt19937 gen (dev());
	ident.runImages(*leftImage, *rightImage, 1);

	std::cout << "finding a new right image" << std::endl;

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return ident.getSkill(*a).mu < ident.getSkill(*b).mu;
				});

	const int tenPercent = (filtered.size() - 1) / 10;

	auto climbBottom = std::find( filtered.begin(), filtered.end(), leftImage) - tenPercent;
	auto climbTop = climbBottom + (tenPercent * rightStreak);

	if(climbBottom < filtered.begin()) climbBottom = filtered.begin();
	if(climbTop >= filtered.end()) climbTop = filtered.end();
	if(climbBottom >= filtered.end() - 20) climbBottom = (climbTop - 25 + leftStreak);
	if(climbBottom >= filtered.end() - 5) climbBottom = (climbTop - 5);

	if( leftStreak == 10) return freshImages();

	leftStreak += 1;
	rightStreak = 0;

	std::uniform_int_distribution<int> dist (0, std::abs(climbTop - climbBottom));

	while(true)
	{
		rightImage = *(climbBottom + dist(gen));
		if(leftImage != rightImage)
			return;
	}

}

void CollectionMan :: rightVictory (void)
{
	std::random_device dev;
	std::mt19937 gen (dev());
	ident.runImages(*leftImage, *rightImage, 2);

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return ident.getSkill(*a).mu < ident.getSkill(*b).mu;
				});

	const int tenPercent = (filtered.size() - 1) / 10;


	auto climbBottom = std::find( filtered.begin(), filtered.end(), rightImage) - tenPercent;
	auto climbTop = climbBottom + (tenPercent * rightStreak);

	if(climbBottom < filtered.begin()) climbBottom = filtered.begin();
	if(climbTop >= filtered.end()) climbTop = filtered.end();
	if(climbBottom >= filtered.end() - 20) climbBottom = (climbTop - 25 + leftStreak);
	if(climbBottom >= filtered.end() - 5) climbBottom = (climbTop - 5);

	if( rightStreak == 10) return freshImages();

	leftStreak = 0;
	rightStreak += 1;


	std::uniform_int_distribution<int> dist (0, std::abs(climbTop - climbBottom));

	while(true)
	{
		leftImage = *(climbBottom + dist(gen));
		if(leftImage != rightImage)
			return;
	}

}

void CollectionMan :: tieVictory (void)
{
	ident.runImages(*leftImage, *rightImage, 3);
	freshImages();
}

void CollectionMan :: setRightImage ( std::shared_ptr< Image> i) 
{
	rightImage = i;
	rightImage = matchingImage(i);
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


std::shared_ptr< Image> CollectionMan :: matchingImage (std::shared_ptr< Image> i)
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

	if(climbTop >= filtered.end()) climbTop = filtered.end();
	if(climbBottom >= filtered.end() - 20) climbBottom = (climbTop - 25 + leftStreak);
	if(climbBottom >= filtered.end() - 5) climbBottom = (climbTop - 5);


	leftStreak = 0;
	rightStreak = 0;

	std::uniform_int_distribution<int> dist (0, std::abs(climbTop - climbBottom));


	while(true)
	{
		out = *(climbBottom + dist(gen));
		if(out != i)
			return out;
	}
}

}


