#include <iostream>
#include "booruInterface.h"
#include "image.h"
#include "metaData.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main(int argc, char *const argv[])
{
	if(argc != 3) {
		std::cout<< "Usage: " << argv[0] << " file\n";
		return 1;
	}


	for(auto& p: fs::recursive_directory_iterator(argv[1])){

		if (p.path().extension() == ".jpg" || p.path().extension()  == ".png"){


		cute::MetaData loop(p.path());
		loop.readTags();


		if(	loop.hasTag(argv[2])){

			std::cout<<"FOUND TAG! "<<argv[1]<<" in directory "<<p.path()<<'\n';
		}

		
	}

	}



	
	return 0;

}

