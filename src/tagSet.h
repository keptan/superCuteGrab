#pragma once 
#include <set> 
#include <string>
#include <map>
#include <memory>
#include <algorithm> 

#include "tag.h" 

namespace cute	{

class TagSet 
{
	std::set<Tag> res; 

	TagSet (const std::set<Tag>&);

	public:
	TagSet (void);
	TagSet (const TagSet&);

	TagSet& operator += (const Tag t); 
	friend TagSet operator & (const TagSet&, const TagSet&);
	friend TagSet operator | (const TagSet&, const TagSet&);
	friend TagSet operator + (const TagSet&,  const TagSet&);

	std::set<Tag>::const_iterator begin (void) const;
	std::set<Tag>::const_iterator end   (void) const;

	size_t size (void) const;
};

				}



