#include "tagSet.h"

namespace cute {

TagSet :: TagSet (void)
{}

TagSet :: TagSet (const Tag& t)
	: res({t})
{}

TagSet :: TagSet (const TagSet& t)
	: res(t.res)
{}

TagSet :: TagSet (const std::set<Tag>& set)
	: res(set)
{}

TagSet& TagSet :: operator += (const Tag t)
{
	res.insert(t);
	return *this;
}

TagSet& TagSet :: operator += (const TagSet& t)
{
	res.insert(t.begin(), t.end());
	return *this;
}

TagSet operator & (const TagSet& left, const TagSet& right)
{
	std::set<Tag> temp; 

	std::set_intersection( 
			left.begin(), left.end(),
			right.begin(), right.end(), 
			std::inserter(temp, temp.begin()));

	return TagSet(temp);
}

TagSet operator | (const TagSet& left, const TagSet& right)
{
	std::set<Tag> temp; 

	std::set_difference( 
			left.begin(), left.end(),
			right.begin(), right.end(), 
			std::inserter(temp, temp.begin()));

	return TagSet(temp);
}

TagSet operator + (const TagSet& left, const TagSet& right)
{
	std::set<Tag> temp = left.res; 

	temp.insert(right.begin(), right.end());

	return TagSet(temp); 
}

std::set<Tag>::const_iterator TagSet :: begin (void) const
{
	return res.begin();
}

std::set<Tag>::const_iterator TagSet :: end (void) const
{
	return res.end(); 
}

size_t TagSet :: size (void) const
{
	return res.size();
}

	}
