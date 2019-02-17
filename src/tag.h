#pragma once 

#include <string> 
#include <vector>

namespace cute {

struct Tag 
{
	std::string tag; 

	Tag (const std::string&);
	Tag (const Tag&);
	bool operator < (const Tag& a) const; 
};

struct SkillDatum 
{
	double mu; 
	double sigma; 

	double skill (void) const;

	SkillDatum (const double m = 100, const double s = 33);
	SkillDatum (const std::vector<double>& );

};

}
