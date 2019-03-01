#pragma once 

#include <filesystem>
#include <map>
#include <fstream>
#include <string>
#include "tagSet.h"
#include <mutex>
#include "image.h"

namespace cute		{

class ScoreDB 
{

	using Hash = PathMetaData::Hash;

	const std::filesystem::path dbFile;
	std::map< Tag, SkillDatum> scoreMap;
	const double decay;


	public:
	ScoreDB (std::filesystem::path, const double d = 0.1);
	~ScoreDB ();

	void readCSV (void);
	void writeCSV (void);

	SkillDatum retrieveData (const Tag&);
	void insertScore (const Tag&, const SkillDatum& = SkillDatum());
};

}
