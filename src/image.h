#pragma once 

#include <filesystem>
#include <string>
#include <memory>

namespace cute {

struct PathMetaData 
{
	using Hash = std::string;
	unsigned long  file_size;
	unsigned long  write_time;
	
	Hash hash;

	PathMetaData (unsigned long , unsigned long, const Hash&);
	PathMetaData (void);

};

struct Image 
{
	const std::filesystem::path location;
	const PathMetaData pData; 

	Image (const std::filesystem::path l, const PathMetaData p);
	Image (void) {};
};


using SharedImage = std::shared_ptr< Image>; 
}





	
	


