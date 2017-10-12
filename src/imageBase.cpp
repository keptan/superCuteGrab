#include "imageBase.h"
#include <experimental/filesystem>
#include <experimental/random>
#include "metaData.h"
#include "skillBase.h"
#include <iostream>
#include "math.h"

namespace cute
{
	ImageBase :: ImageBase(std::string l,SkillBase *s,FileBase *f)
		: loc(l),scrolledTo(0),skillBase(s),fileBase(f)
	{}

	int ImageBase :: readDirectory(int i)
	{
		int added;

		for(auto& p: fs::recursive_directory_iterator(loc)){

			std::string e = p.path().extension();

			if (e == ".jpg" || e == ".png" ||e == "jpeg"){

				localImages.push_back(MetaData(p.path()));
				added++;
				scrolledTo++;
			}

			if ( added > i & i > 0)
				return added;
		}
	}
	int ImageBase :: readDirectory(std::string l)
	{
		int added;

		for(auto& p: fs::recursive_directory_iterator(l)){

			std::string e = p.path().extension();

			if (e == ".jpg" || e == ".png" ||e == "jpeg"){

				localImages.push_back(MetaData(p.path()));
				added++;
				scrolledTo++;
			}

			}
	
	}

	int ImageBase :: scrollDirectory(int i)
	{
		auto viter = localImages.begin();

		localImages.erase(viter,viter+i);
		int passed;


		for(auto& p: fs::recursive_directory_iterator(loc)){

		if(i-- < 0)
			return i;
			
		passed++;

		if( passed > i){
			std::string e = p.path().extension();

			if (e == ".jpg" || e == ".png" ||e == "jpeg"){

				localImages.push_back( MetaData(p.path()));
				scrolledTo++;
				i--;
			}

		}
		}


	}

	int ImageBase :: filter(std::string t)
	{
		filteredImages.clear();
		fileBase->clear();



	
		if (t.length() == 0){
			for(auto i : localImages)
				filteredImages.push_back(i);

			return 0;
		}
		for(auto &i : localImages){
			if(!i.read){
			i.readTags();
			i.read = true;
			}
			if(i.hasTag(t)){

				i.tempMu = skillBase->getTag(i).getMu();
				i.tempSigma = skillBase->getTag(i).getSigma();
				filteredImages.push_back(i);

				fileBase->addTag(i);
			}



			}
		std::sort( filteredImages.begin( ), filteredImages.end( ), [ ]( const auto& lhs, const auto& rhs )
		{
	  // return ((lhs.tempMu * lhs.tempMu * lhs.tempMu)/ lhs.tempSigma )> (rhs.tempMu * rhs.tempMu * rhs.tempMu)/ rhs.tempSigma ;
	   return lhs.tempMu > rhs.tempMu ;
		});
	fileBase->writeFile();
	fileBase->readFile();
	}

	MetaData ImageBase :: findMatch(MetaData m,int c )
	{
		int currentElo = skillBase->getTag(m).getMu();
		int currentSigma = skillBase->getTag(m).getSigma();

		FileDatum nextDatum = fileBase->getRandom();
		MetaData nextData(nextDatum.getLoc());
		SkillDatum nextSDatum = skillBase->getTag(nextData);

		int nextElo = nextSDatum.getMu();
		int nextSigma = nextSDatum.getSigma();



		int var = 0 + (c*2);
		if(c < 0)
			var = 30;

		int iter = 0;
		int siter = 0;

		while(true){
		if(nextData.fileName() != m.fileName()){

			if(c < 0){
				if ((nextSigma >= var)){
				nextData.readTags();
				return nextData;
				}

			}
			if(c >= 0){

			if (( nextElo - currentElo) >= var && (nextElo - currentElo) <= (c * 3 )+ siter / 120 ){

				nextData.readTags();
				return nextData;
			}

			}

			if (siter > 1000){

				nextData.readTags();
				return nextData;
			}
			}
			iter++;
			siter++;
			if(iter > 50)
			{
				iter = 0;
				var -= 1;
			}

		nextDatum = fileBase->getRandom();
		nextData = MetaData(nextDatum.getLoc());
		nextSDatum = skillBase->getTag(nextData);

		nextElo = nextSDatum.getMu();
		nextSigma = nextSDatum.getSigma();
		std::cout<<nextElo<<" "<<nextSigma<<'\n';


		}
		return m;
	}

	void ImageBase :: runElo(MetaData w, MetaData l,int t)
	{

	skillBase->readFile();

	cute::SkillHandle sh(w,l,skillBase,t);
	sh.run();
	sh.setTags();

	skillBase->writeFile();

	}


		


	std::vector<MetaData> *ImageBase :: collectImages()
	{
		return &filteredImages;
	}
}

