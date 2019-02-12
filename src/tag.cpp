#include "tag.h" 
#include <utility>
#include <tuple>

namespace cute {

Tag :: Tag (const std::string& t)
	: tag(t)
{}

Tag :: Tag (const Tag& t)
	: tag(t.tag)
{}

bool Tag :: operator < (const Tag& a) const 
{
	return tag < a.tag;
}

SkillDatum :: SkillDatum ( const double m, const double s)
	: mu(m), sigma(s)
{}

SkillDatum :: SkillDatum ( const std::vector<double>& p)
{
	if(p[0] > 5)
		mu = p[0];
	else
		mu =5; 

	if (p[1] > 1)
		sigma = p[1];
	else 
		sigma = 1;
}

double SkillDatum :: skill (void)
{
	return mu - (3 * sigma); 
}

				}
