#include "tagDB.h" 
#include <iostream>
#include <iomanip>

namespace cute	{

TagDB :: TagDB (std::filesystem::path p)
	:dbFile(p)
{
	readCSV();
}

TagDB :: ~TagDB (void)
{
	writeCSV();
}

void TagDB :: readCSV (void)
{
	std::scoped_lock lk (fileMutex, entryMutex);

	std::fstream fs(dbFile); 
	fs.seekg(0, std::ios::beg);

	std::string line; 

	while( std::getline(fs, line))
	{


		std::string head; 
		std::string acc;
		TagSet tags;
		auto i = line.begin();

		if(line.size() < 3) continue;

		for(; *i != ',' ;i++)
		{
			head += *i;
		}
		i++;

		bool inQuote = false;

		for(; i != line.end(); i++)
		{
		
			if(*i == ',')
			{
				tags += acc;
				acc = "";
				continue;
			}

			acc += *i;
		}

		tags += acc;

		tagMap.insert( std::make_pair(head, tags));

	}

}

void TagDB :: writeCSV (void)
{
	std::shared_lock sl (entryMutex);
	std::scoped_lock lk (fileMutex);
	std::ofstream fs(dbFile);

	for( const auto& pair : tagMap)
	{
		if(!pair.second.size()) continue;
		std::ostringstream tagString; 
		tagString << pair.first;


		for(const auto& t : pair.second)
		{
			std::string acc = t.tag;
			acc.erase(std::remove_if(acc.begin(), acc.end(), 
						[](const auto c){ if(c == ',' || c == '"') return true; return false;}), acc.end());

			tagString << ',' << acc;
		}



		fs << tagString.str() << '\n';
	}
}

void TagDB :: insertTags (const Hash& h, const TagSet& t)
{
	std::scoped_lock lk (entryMutex);
	const auto it = tagMap.find(h);

	if(it == tagMap.end())
	{
		tagMap.insert( std::make_pair( h, t));
		return;
	}
	
	it->second += t;
}
			

void TagDB :: clearTags (const Hash& h)
{	
	std::scoped_lock lk (entryMutex);
	const auto it = tagMap.find(h);
	if(it == tagMap.end())
	{
		return;
	}
	
	tagMap.erase(it);
}
	

TagSet TagDB :: retrieveData (const Hash& h) const
{
	std::shared_lock lk (entryMutex);
	const auto f = tagMap.find(h);

	if(f == tagMap.end()) return TagSet();

	return f->second; 
}

TagSet TagDB :: retrieveData (void) const
{
	std::shared_lock lk (entryMutex);
	TagSet tags; 

	for(const auto t: tagMap)
	{
		tags += t.second;
	}

	return tags;
}

			}
