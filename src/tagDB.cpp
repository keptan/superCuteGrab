#include "tagDB.h" 
#include <iostream>
#include <iomanip>

namespace cute	{

TagDB :: TagDB (std::filesystem::path p)
	:dbFile(p)
{
	readCSV();
}

void TagDB :: readCSV (void)
{
	std::fstream fs(dbFile); 
	fs.seekg(0, std::ios::beg);

	std::string line; 

	while( std::getline(fs, line))
	{
		std::istringstream is(line); 

		std::string h, t; 

		is >> std::quoted(h) >> std::quoted(t);

		std::istringstream tags(t);
		std::string tag1, tag2;
		//weewooweeewoo
		tags >> std::quoted(tag1) >> std::quoted(tag2);

	}
}

void TagDB :: writeCSV (void)
{
	std::ofstream fs(dbFile);

	for( auto& pair : tagMap)
	{
		std::ostringstream tagString; 

		for(auto& t : pair.second)
			tagString << std::quoted(t.tag) <<  '	';

		fs << std::quoted(pair.first) << '	' << std::quoted(tagString.str()) << '\n';
	}
}

void TagDB :: insertTags (const Hash& h, const TagSet& t)
{
	const auto it = tagMap.find(h);

	if(it == tagMap.end())
	{
		tagMap.insert( std::make_pair( h, t));
		return;
	}
	
	it->second = t;
}
			
	

TagSet TagDB :: retrieveData (const Hash& h)
{
	return tagMap.find(h)->second; 
}

			}
