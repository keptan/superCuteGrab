#include <iostream>
#include "booruInterface.h"
#include "image.h"
#include "metaData.h"
#include "ThreadPool/ThreadPool.h"
#include "skillBase.h"

#include <curl/curl.h>
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

	cute::SkillBase sb("dataBase");
	int found = 0;
	std::vector<std::string> p1;
	std::vector<std::string> p2;

	for(auto& p: fs::recursive_directory_iterator(p)){

		if (p.path().extension() == ".jpg" || p.path().extension()  == ".png"){


		cute::MetaData loop(p.path());
		loop.readTags();


		if(	loop.tagged()){

			std::cout<<"found tagged" << " in directory "<<p.path()<<'\n';
			if(found == 0)
			p1 = loop.getTags();
			else
			p2 = loop.getTags();


			found++;
			if (found > 1)
				break;
			}
		}
		
	}

	std::cout<<"creating skillhandle\n";
	cute::SkillHandle sh(p1,p2,&sb);
	std::cout<<"running skillhandle\n";
	sh.run();
	std::cout<<"setting tags\n";
	sh.setTags();

	std::cout<<"writing to file\n";
	sb.writeFile();




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

int danTest(std::string p)
{

	std::cout<<"initing curl.....";

	curl_global_init(CURL_GLOBAL_ALL);

	std::cout<<"inited"<<std::endl;
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
				interfaces.back()->makeHash();
			}
		}else{
			w++;
		}
	}

	for(auto i : interfaces){
		if(!i->tagged()){
			results.emplace_back(pool.enqueue([&]{

						runDoc(i,0);
						return 1;
				}));
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

	if(argv[1] == (std::string) "skillTest")
	skillTest(argv[2]);

	else if(argv[1] == (std::string)"danTest")
	danTest(argv[2]);




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





