#include <iostream>
#include "booruInterface.h"
#include "image.h"
#include "metaData.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main(int argc, char *const argv[])
{

	cute::Image file(argv[1]);
	//std::cout << file.getHash() << std::endl;

	cute::BooruInterface danSearch(file.getHash());

	danSearch.getDoc();
	danSearch.readTags();
	danSearch.printTags();


	
	return 0;

}

int directoryScan(std::string p, std::string t)
{
	for(auto& p: fs::recursive_directory_iterator(p)){

		if (p.path().extension() == ".jpg" || p.path().extension()  == ".png"){


		cute::MetaData loop(p.path());
		loop.readTags();


		if(	loop.hasTag(t)){

			std::cout<<"FOUND TAG! "<<t<<" in directory "<<p.path()<<'\n';
		}

		
	}

	}
	return 0;


}

