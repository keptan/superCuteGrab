#pragma once 

#include <filesystem> 
#include <map>
#include <fstream> 
#include <string>
#include "image.h"
#include "tagSet.h" 

namespace cute	{

class TagDB
{
	using Hash = PathMetaData::Hash; 

	const std::filesystem::path dbFile;
	std::map< Hash, TagSet> tagMap; 

public: 
	TagDB (std::filesystem::path); 
	~TagDB (void);

	void readCSV  (void); 
	void writeCSV (void);
//	bool contains (const Hash&); 
	TagSet retrieveData (const Hash&);
	TagSet retrieveData (void);
	void insertTags (const Hash&, const TagSet&);
	void clearTags  (const Hash&);
};



				}
