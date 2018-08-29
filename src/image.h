#pragma once 

#include <filesystem>
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

struct Image 
{
	const std::filesystem::path location;
	const PathMetaData pData; 

	Image (const std::filesystem::path l, const PathMetaData p);
};
}





	
	


