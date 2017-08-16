#include <iostream>
#include "booruInterface.h"
#include "image.h"
#include "metaData.h"
#include "trueskill/mathexpr.h"
#include "trueskill/trueskill.h"
#include "threadPool/threadPool.h"

#include <experimental/filesystem>
#include <vector>
#include <mutex>
#include <thread>


static std::mutex m;
static int threadsDone = 0;
static int totalThreads =0;
static int numThreads = 0;

namespace fs = std::experimental::filesystem;

void runDoc(cute::BooruInterface *in)
{
	m.lock();
	numThreads++;
	totalThreads++;
	m.unlock();

	in->getDoc();

	m.lock();
	threadsDone++;
	numThreads--;
	std::cout<<"receiving curl threads "<< threadsDone << '/' << numThreads << " lifetime threads: " << totalThreads<<"                        "<<'\r';
	m.unlock();

}

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

	std::cout<<"scanning directory " << p <<  std::endl;

	int i = 0;
	int w = 0;
	int f = 0;
	std::cout<<'\n';


	//std::vector<cute::MetaData *> files;
	std::vector<cute::BooruInterface *> interfaces;
	std::vector<std::thread *> threads;

	std::vector<std::future<int> > results;


	ThreadPool pool(64);


	for(auto& p: fs::recursive_directory_iterator(p)){

		i++;

		std::string e = p.path().extension();

		if (e == ".jpg" || e == ".png" ||e == "jpeg"){


			interfaces.push_back(new cute::BooruInterface(p.path()));

			interfaces.back()->readTags();


			if(!interfaces.back()->tagged()){


				//threads.push_back(new std::thread(runDoc,interfaces.back()));
				//
				
				results.emplace_back(pool.enqueue([&]{

						runDoc(interfaces.back());
						return 1;
				}));


			

			}
			else{
				w++;
			}
		}
	}



	for(auto && result: results){
		result.get();
	}



	std::cout<<"\nthreads done, tagged: \n";
	for( auto n :interfaces){

		if(n->readDocTags()){
			f++;
			std::cout<<n->fileName()<<' ';
			n->printDocTags();
			std::cout<<"...\n";

		}


	}

	std::cout<<f<<" images";



	std::cout<<std::endl;
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





