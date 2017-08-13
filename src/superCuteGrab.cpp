#include <iostream>
#include "booruInterface.h"
#include "image.h"
#include "metaData.h"
#include "trueskill/mathexpr.h"
#include "trueskill/trueskill.h"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


int rankTest()
{
	mathexpr_sanity_check();

	Player* player1 = new Player();
	player1->mu = 25.0;
	player1->sigma = 25.0/3.0;

	Player* player2 = new Player();
	player2->mu = 25.0;
	player2->sigma = 25.0/3.0;

	std::vector<Player*> players;
	players.push_back(player1);
	players.push_back(player2);

	player1->rank = 2;
	player2->rank = 2;

	TrueSkill ts;
for(int i = 0; i<25;i++){
	ts.adjust_players(players);


  std::cout << "player1: " << player1->mu << " | " << player1->sigma << std::endl;
  std::cout << "player2: " << player2->mu<< " | " << player2->sigma <<  std::endl;



}
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

int booruWriteScan(std::string p)
{
	for(auto& p: fs::recursive_directory_iterator(p)){

		if (p.path().extension() == ".jpg" || p.path().extension()  == ".png"){




		cute::MetaData file(p.path());
		file.readTags();

		if(!file.tagged()){


		cute::BooruInterface danSearch(file.getHash());

		danSearch.getDoc();
		if(danSearch.readTags()){

		std::cout << "writing tags to " << p.path << std::endl;

		file.addTag("MD5:" + file.getHash());

		for (auto element : danSearch.getTags()){
			file.addTag(element);
		}

		file.writeTags();
		}
		}
			
	}

	}
	return 0;


}

int scanPreviouslyTagged(std::string p)
{
	for(auto& p: fs::recursive_directory_iterator(p)){

		if (p.path().extension() == ".jpg" || p.path().extension()  == ".png"){


		cute::MetaData loop(p.path());
		loop.readTags();


		if(	loop.tagged()){

			std::cout<<"found tagged" << " in directory "<<p.path()<<'\n';
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
	scanPreviouslyTagged(argv[1]);
	booruWriteScan(argv[1]);
	





/*	cute::Image file(argv[1]);
	//std::cout << file.getHash() << std::endl;

	cute::BooruInterface danSearch(file.getHash());

	danSearch.getDoc();
	if(danSearch.readTags())
		std::cout<<"tag found!"<<std::endl;

	//danSearch.printTags();
*/

//	booruScan(argv[1]);


	
	return 0;
}





