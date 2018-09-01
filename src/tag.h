#pragma once 

#include <string> 
#include <vector>

namespace cute {

struct Tag 
{
	const std::string tag; 

	Tag (const std::string&);
	bool operator < (const Tag& a) const; 
};

struct SkillDatum 
{
	double mu; 
	double sigma; 

	SkillDatum (const double m = 100, const double s = 33);
	SkillDatum (const std::vector<double>& );
};

}
