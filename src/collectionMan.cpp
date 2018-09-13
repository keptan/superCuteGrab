#include "collectionMan.h" 

namespace cute  {

CollectionMan :: CollectionMan (IdentityRank& i, std::vector< std::shared_ptr< Image>> c)
	: ident(i), collection(c), filtered(c), leftStreak(0), rightStreak(0) 
{
	//freshImages();
}

void CollectionMan :: setImages (const std::vector< std::shared_ptr<Image>> i)
{
	filtered = i;
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

	ident.saveTags();
	std::random_device dev;
	std::mt19937 gen (dev());

	std::sort (filtered.begin(), filtered.end(), 
				[&](const auto a, const auto b) {
					return ident.getSkill(*a).sigma > ident.getSkill(*b).sigma;
				});
	leftImage  = *(filtered.begin());
	rightImage = matchingImage(leftImage);
}
	
void CollectionMan :: leftVictory (void)
{

	ident.runImages(*leftImage, *rightImage, 1);

	leftStreak++;
	rightStreak = 0;

	if(leftStreak >= 10)
		return freshImages();

	//rightImage = matchingImage(leftImage, leftStreak);
	rightImage = matchingELO( leftImage, leftStreak);
	std::cout << "left ELO is currently: " << ident.getSkill(*leftImage).skill()   << std::endl;
	std::cout << "right ELO is currently: " << ident.getSkill(*rightImage).skill() << std::endl;

}

void CollectionMan :: rightVictory (void)
{

	ident.runImages(*leftImage, *rightImage, 2);

	leftStreak = 0;
	rightStreak++;

	if(rightStreak >= 10)
		return freshImages();

//	leftImage= matchingImage( rightImage, rightStreak);
	leftImage = matchingELO( rightImage, rightStreak);
	std::cout << "left ELO is currently: " << ident.getSkill(*leftImage).skill() << std::endl;
	std::cout << "right ELO is currently: " << ident.getSkill(*rightImage).skill() << std::endl;

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


	const int twoPercent = std::max<int>( ((filtered.size() - 1  ) / 20), 5);

	auto climbBottom = std::find( filtered.begin(), filtered.end(), i) - twoPercent;
	auto climbTop = climbBottom + (twoPercent * (streak / 2 + 1));


	if(climbTop >= filtered.end()) climbTop = filtered.end() - 1;
	if(climbBottom < filtered.begin()) climbBottom = filtered.begin(); 

	if(climbBottom >= filtered.end() - 15) climbBottom = filtered.end() - 15;


	std::cout << "matching image between ";
	std::cout << ident.getSkill(**(climbBottom)).skill() << " and ";
	std::cout << ident.getSkill(**(climbTop)).skill() << std::endl;



	std::uniform_int_distribution<int> dist (0, std::abs(climbTop - climbBottom));

	while(true)
	{
		out = *(climbBottom + dist(gen));
		if(out != i)
			return out;
	}
}






}


