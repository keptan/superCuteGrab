#include "hashDB.h" 
#include "fileMd5.h"
#include <iomanip>
#include <cassert>
#include <iostream>
#include <set>

namespace cute {


HashDB :: HashDB (std::filesystem::path p)
		:dbFile(p)
{
		readCSV();
}
void HashDB :: readCSV (void)
{
	std::fstream fs;
	fs.open(dbFile);
	fs.seekg(0, std::ios::beg);

	std::string line;

	while(std::getline(fs, line))
	{
		std::istringstream is(line);

		unsigned long t, s;
		std::string p, h;

		is >> std::quoted(p) >> s >> t >> h;
		pathMap.insert( std::make_pair( p, PathMetaData(s, t, h)));
	}

	fs.close();
}

void HashDB :: writeCSV (void)
{
	std::ofstream fs;
	fs.open(dbFile);

	for(auto pair : pathMap)
	{
		fs << pair.first << '	' << pair.second.file_size << '	' << pair.second.write_time << '	' << pair.second.hash << '\n';
	}
		

	fs.close();
}

void HashDB :: scanDirectory (std::filesystem::path p)
{
	for(auto& f : std::filesystem::directory_iterator(p))
	{

		if(!conformingFileType(f.path())) continue;

		const auto rawTime = std::filesystem::last_write_time(f);
		const auto epoch = rawTime.time_since_epoch();
		const auto value = std::chrono::duration_cast<std::chrono::seconds>(epoch);

		const unsigned long time = value.count();
		const unsigned long size = std::filesystem::file_size(f);
		const auto absolute = std::filesystem::absolute(f.path());

		const auto it = pathMap.find(absolute);
		
		//if our file does contain our path
		if(it != pathMap.end())
		{
			//if the store metadata is still accurate, we add it to our localPathMap
			if(it->second.file_size == size && it->second.write_time == time)
			{
				localPathMap.insert_or_assign(absolute, it->second);

				std::cout << "using stored metadata for " << f.path().string() << '\n'; 
				continue;
			}
		}
		std::cout << "stored metadata for " << f.path().string() << " was no longer valid\n"; 

		//if the stored metadata is no longer accurate we replace the store metadata and rehash the file
		const Hash hash = hashFile(absolute);

		pathMap.insert_or_assign( absolute,  PathMetaData(size, time, hash));
		localPathMap.insert_or_assign(absolute, PathMetaData(size, time, hash));
	}

	writeCSV();
}

bool HashDB :: contains (const std::filesystem::path p)
{
	const auto absolute = std::filesystem::absolute(p);

	return localPathMap.find(absolute) != localPathMap.end();
}

PathMetaData HashDB :: retrieveData (const std::filesystem::path p)
{
	const auto absolute = std::filesystem::absolute(p);

	assert (contains(absolute));
	return localPathMap.find(absolute)->second;
}

}
