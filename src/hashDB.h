
#ifndef HASH_DB 
#define HASH_DB 

#include <filesystem> 
#include <map>
#include <fstream>
#include <string>
#include "image.h"
#include "filetypes.h"
#include <mutex>
#include <shared_mutex>


namespace cute {



class HashDB
{

	using Hash = PathMetaData::Hash;

	std::filesystem::path dbFile;
	std::map<std::filesystem::path, PathMetaData> pathMap;
	std::map<std::filesystem::path, PathMetaData> localPathMap;

	mutable std::shared_mutex entryMutex;
	mutable std::mutex		  fileMutex;

	void writeCSV (void);

public:
	HashDB (std::filesystem::path );
	~HashDB (void);

	void readCSV (void);
	
	void scanDirectory (std::filesystem::path p);
	void scanDirectoryRecursive (std::filesystem::path p);

	bool contains (const std::filesystem::path p) const;
	PathMetaData retrieveData (const std::filesystem::path p) const;

	std::map<std::filesystem::path, PathMetaData>::const_iterator  begin (void) const;
	std::map<std::filesystem::path, PathMetaData>::const_iterator  end   (void) const;

};
}
#endif


