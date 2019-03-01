#pragma once 

#include <filesystem> 
#include <map>
#include <fstream> 
#include <string>
#include <shared_mutex>
#include <mutex>
#include "image.h"
#include "tagSet.h" 

namespace cute	{

class TagDB
{
	using Hash = PathMetaData::Hash; 

	const std::filesystem::path dbFile;
	std::map< Hash, TagSet> tagMap; 
	mutable std::shared_mutex entryMutex;
	mutable std::mutex		  fileMutex;

public: 
	TagDB (std::filesystem::path); 
	~TagDB (void);

	void readCSV  (void); 
	void writeCSV (void);
//	bool contains (const Hash&); 
	TagSet retrieveData (const Hash&) const;
	TagSet retrieveData (void) const;
	void insertTags (const Hash&, const TagSet&);
	void clearTags  (const Hash&);
};



				}
