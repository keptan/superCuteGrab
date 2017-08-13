#include <iostream>
#include "booruInterface.h"
#include "image.h"
#include "metaData.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


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

int booruScan(std::string p)
{
	int i = 0;

	for(auto& p: fs::recursive_directory_iterator(p)){

		if (p.path().extension() == ".jpg" || p.path().extension()  == ".png"){


		cute::Image loop(p.path());
		cute::BooruInterface interface(loop.getHash());

		interface.getDoc();


		if(	interface.readTags()){

			std::cout<<"FOUND TAGS! "<<" in directory "<<p.path()<<'\n';
			i++;
		}else
			std::cout<<"file scanned"<<std::endl;

		
	}

	}
	std::cout<<std::endl<<"total found: "<<i<<std::endl;
	return 0;


}

int main(int argc, char *const argv[])
{

/*	cute::Image file(argv[1]);
	//std::cout << file.getHash() << std::endl;

	cute::BooruInterface danSearch(file.getHash());

	danSearch.getDoc();
	if(danSearch.readTags())
		std::cout<<"tag found!"<<std::endl;

	//danSearch.printTags();
*/

	booruScan(argv[1]);


	
	return 0;
}





