#include <iostream>
#include "booruInterface.h"
#include "image.h"
#include "metaData.h"
#include "ThreadPool/ThreadPool.h"
#include "skillBase.h"
#include "imageBase.h"

#include <openssl/crypto.h>
#include "graphics/searchWindow.h"

#include <curl/curl.h>
#include <experimental/filesystem>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <gtkmm.h>


static std::mutex m;
static int threadsDone = 0;
static int totalThreads =0;
static int numThreads = 0;

namespace fs = std::experimental::filesystem;

static pthread_mutex_t *lockarray;

static void lock_callback(int mode, int type, char *file, int line)
{
  (void)file;
  (void)line;
  if (mode & CRYPTO_LOCK) {
    pthread_mutex_lock(&(lockarray[type]));
  }
  else {
    pthread_mutex_unlock(&(lockarray[type]));
  }
}

static unsigned long thread_id(void)
{
  unsigned long ret;

  ret=(unsigned long)pthread_self();
  return(ret);
}

static void init_locks(void)
{
  int i;

  lockarray=(pthread_mutex_t *)OPENSSL_malloc(CRYPTO_num_locks() *
                                        sizeof(pthread_mutex_t));
  for (i=0; i<CRYPTO_num_locks(); i++) {
    pthread_mutex_init(&(lockarray[i]),NULL);
  }

  CRYPTO_set_id_callback((unsigned long (*)())thread_id);
  CRYPTO_set_locking_callback((void (*)(int, int, const char*, int))lock_callback);
}

static void kill_locks(void)
{
  int i;

  CRYPTO_set_locking_callback(NULL);
  for (i=0; i<CRYPTO_num_locks(); i++)
    pthread_mutex_destroy(&(lockarray[i]));

  OPENSSL_free(lockarray);
}

void skillTest(std::string p)
{
	/*
	cute::SkillBase sb("dataBase");
	int found = 0;
	std::vector<std::string> p1;
	std::vector<std::string> p2;


	std::cout<<"creating skillhandle\n";
	cute::SkillHandle sh(p1,p2,&sb);
	std::cout<<"running skillhandle\n";
	sh.run();
	std::cout<<"setting tags\n";
	sh.setTags();

	std::cout<<"writing to file\n";
	sb.writeFile();

	*/


}


void gtkTest(std::string l)
{




	cute::SkillBase sBase("database");
	cute::ImageBase base(l,&sBase);
	base.readDirectory();

	Gtk::Main kit(NULL,NULL);
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("window.glade");

	cute::SearchWindow *sw = 0;
	builder->get_widget_derived("SearchWindow",sw);
	sw->baseInit(&base);

	sw->show_all();
	kit.run(*sw);

	
	return;

}


void runDoc(cute::BooruInterface *in,int depth)
{
	m.lock();
	numThreads++;
	totalThreads++;
	m.unlock();

	in->getDoc();
	

	
	if(in->reCurl() == -1 && depth < 3){
		std::this_thread::sleep_for(std::chrono::seconds(4));
		runDoc(in,depth+1);
	}
	

	m.lock();
	threadsDone++;
	numThreads--;
	//std::cout<<"receiving curl threads "<< threadsDone << '/' << numThreads << " lifetime threads: " << totalThreads<<"                        "<<'\r';
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

	init_locks();
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
	std::cout<<"scanning "<< p.path().filename() << std::endl;
				//
				interfaces.back()->makeHash();
				runDoc(interfaces.back(),1);
				if(interfaces.back()->read())
				std::cout<<"Tags found! "<< std::endl;
			}
		}else{

	std::cout<<"scanning "<< p.path().filename() << "already tagged!" <<std::endl;
			w++;
		}
	}

	//std::cout<<"starting boorusearch "  <<  std::endl;

	/*
	for(auto i : interfaces){
		if(!i->tagged()){
			results.emplace_back(pool.enqueue([&]{

						runDoc(i,0);
						return 1;
				}));
		}
		}

		*/

			



	/*
	for(auto && result: results){
		result.get();
	}
	*/



	std::cout<<"\nthreads done, tagged: \n";
	for( auto n :interfaces){

		if(!(n->readDocTags())){
			f++;
			std::cout<<n->fileName()<<' ';
			n->printDocTags();
			std::cout<<"...\n";
			n->writeDocTags();


		}


	}

	std::cout<<f<<" images";



	std::cout<<std::endl;
	kill_locks();
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

int baseTest(std::string l)
{
	/*
	cute::ImageBase base(l);
	base.readDirectory();
	*/


}




int main(int argc, char *const argv[])
{
//	booruWriteScan(argv[1]);

	if(argv[1] == (std::string) "skillTest")
	skillTest(argv[2]);

	else if(argv[1] == (std::string)"danTest")
	danTest(argv[2]);
	
	else if(argv[1] == (std::string)"gtkTest")
	gtkTest(argv[2]);

	else if(argv[1] == (std::string)"searchTest")
	directoryScan(argv[2],argv[3]);

	else if(argv[1] == (std::string)"baseTest")
	baseTest(argv[2]);



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





