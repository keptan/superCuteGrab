
#ifndef HASH_DB 
#define HASH_DB 

#include <filesystem> 
#include <map>
#include <fstream>
#include <string>


namespace cute {

struct PathMetaData 
{
	using Hash = std::string;
	unsigned long  file_size;
	unsigned long  write_time;
	
	Hash hash;

	PathMetaData (unsigned long , unsigned long, const Hash&);

};

class HashDB
{

	using Hash = std::string;

	std::filesystem::path dbFile;
	std::map<std::filesystem::path, PathMetaData> pathMap;
	std::map<std::filesystem::path, PathMetaData> localPathMap;

public:
	HashDB (std::filesystem::path );

	void readCSV (void);
	void writeCSV (void);
	
	void scanDirectory (std::filesystem::path p);

};
}
#endif


