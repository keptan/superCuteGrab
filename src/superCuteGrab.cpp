#include <iostream>
#include "booruInterface.h"
#include "image.h"
#include "metaData.h"
#include "trueskill/mathexpr.h"
#include "trueskill/trueskill.h"
#include "threadPool/threadPool.h"
#include "skillBase.h"

#include <experimental/filesystem>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>


static std::mutex m;
static int threadsDone = 0;
static int totalThreads =0;
static int numThreads = 0;

namespace fs = std::experimental::filesystem;

void skillTest(std::string p)
{
	cute::SkillBase sb(p);
	if(sb.hasTag("cuteTag")){
		std::cout<<"has cuteTag"<<std::endl;
		std::cout<<"getting tag"<<std::endl;
		cute::SkillDatum datum = sb.getTag("cuteTag");
		std::cout<<"accessing tag: ";
		std::cout<<datum.getName()<<' '<<datum.getMu()<<std::endl;
		datum.setMu(200);
		sb.setTag(datum);
		datum = sb.getTag("cuteTag");
	std::cout<<"accessing tag: ";
		std::cout<<datum.getName()<<' '<<datum.getMu()<<std::endl;





	}



	if(sb.hasTag("fgfgfTag"))
		std::cout<<"has fgfgTag";

}

void runDoc(cute::BooruInterface *in,int depth)
{
	m.lock();
	numThreads++;
	totalThreads++;
	m.unlock();

	in->getDoc();

	
	if(in->readDocTags() == -1 && depth < 3){
		std::this_thread::sleep_for(std::chrono::seconds(1));
		runDoc(in,depth+1);
	}

	m.lock();
	threadsDone++;
	numThreads--;
	std::cout<<"receiving curl threads "<< threadsDone << '/' << numThreads << " lifetime threads: " << totalThreads<<"                        "<<'\r';
	m.unlock();

	return;



}

int rankTest()
{
	mathexpr_sanity_check();

	std::vector<Player*> image1;
	std::vector<Player*> image2;


	Player* player1 = new Player();
	player1->mu = 35.0;
	player1->sigma = 3.0;

	Player* player1a = new Player();
	player1a->mu = 35.0;
	player1a->sigma = 3.0;

	Player* player1b = new Player();
	player1b->mu = 25.0;
	player1b->sigma = 2.0;

	Player* player1c = new Player();
	player1c->mu = 25.0;
	player1c->sigma = 1;

	image1.push_back(player1);
	image1.push_back(player1b);
	image1.push_back(player1c);
	image1.push_back(player1a);


	Player* player2 = new Player();
	player2->mu = 35.0;
	player2->sigma = 1.1;

	Player* player2a = new Player();
	player2a->mu = 15.0;
	player2a->sigma = 1.1;

	image2.push_back(player2);
	image2.push_back(player2a);

	while(image2.size() < image1.size()){
		image2.push_back(new Player());
		image2.back()->mu =( player2->mu+player2a->mu) /2;
		image2.back()->sigma = (player2->sigma+player2a->sigma)/2;

	}



	std::vector<Player*> players;
	for(auto & n : image1){
		players.push_back(n);
		players.back()->rank = 2;

	}

	for(auto & n : image2){
		players.push_back(n);

		players.back()->rank = 1;

	}








	TrueSkill ts;
for(int i = 0; i<20;i++){
	ts.adjust_players(players);


  std::cout << "player1: " << player1->mu << " | " << player1->sigma << std::endl;

  std::cout << "player1a: " << player1a->mu << " | " << player1a->sigma << std::endl;

  std::cout << "player2: " << player2->mu << " | " << player2->sigma << std::endl;

  std::cout << "player2a: " << player2a->mu << " | " << player2a->sigma << std::endl;

  //fill the team with average of current players to avoid player count weighting
  //add to the mu the more tags present

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


	ThreadPool pool(24);


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

						runDoc(interfaces.back(),0);
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

		if(!(n->readDocTags())){
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
//	booruWriteScan(argv[1]);
	skillTest(argv[1]);
//	rankTest();



	





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





