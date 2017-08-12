#include <iostream>
#include "image.h"
#include "metaData.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main(int argc, char *const argv[])
{
	if(argc != 2) {
		std::cout<< "Usage: " << argv[0] << " file\n";
		return 1;
	}


	for(auto& p: fs::recursive_directory_iterator(argv[1])){

		cute::MetaData loop(p.path());
		loop.readTags();


		if(	loop.hasTag(argv[2])){

			std::cout<<"FOUND TAG! "<<argv[1]<<" in directory "<<p.path()<<'\n';
		}

		else
			std::cout<<"scanned file\n";

	}



	
	return 0;

}

