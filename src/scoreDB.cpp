#include "scoreDB.h" 
#include <iostream>
#include <iomanip> 

namespace cute		{

ScoreDB :: ScoreDB (std::filesystem::path p)
	:dbFile(p)
{
	readCSV();
}

ScoreDB :: ~ScoreDB (void)
{
	writeCSV();
}

void ScoreDB :: readCSV (void)
{
	std::fstream fs(dbFile);
	fs.seekg(0, std::ios::beg);

	std::string line; 

	while (std::getline(fs, line))
	{
		std::istringstream is(line);
		std::string t;
		double sigma, mu;

		is >> std::quoted(t) >> mu >> sigma; 

		//decay sigma every boot 
		if(sigma < 12) sigma = std::min<double>( sigma + 3, 12.0);

		scoreMap.insert( std::make_pair( 
				Tag(t), SkillDatum(mu, sigma)));
	}
}

void ScoreDB :: writeCSV (void)
{
	std::ofstream fs(dbFile);

	for (auto& pair : scoreMap)
	{
		fs << std::quoted(pair.first.tag) << '	' 
		<< pair.second.mu << '	' << pair.second.sigma << '\n';
	}
}

SkillDatum ScoreDB :: retrieveData (const Tag& t)
{
	const auto it = scoreMap.find(t);

	if(it == scoreMap.end())
	{
		scoreMap.insert( std::make_pair(t, SkillDatum()));
		return SkillDatum();
	}

	return it->second; 
}

void ScoreDB :: insertScore (const Tag& t, const SkillDatum& s)
{
	//scoreMap.insert_or_assign(t, s);
	scoreMap[t] = s;
}






			}






