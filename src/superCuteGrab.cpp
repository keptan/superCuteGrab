#include <iostream>
#include "hashDB.h"
#include "thumbDB.h"
#include "graphics/window.h"
#include "tagSet.h"
#include "tagDB.h" 
#include "scoreDB.h"
#include "cskill_instance.h"
#include "collectionMan.h"
#include "compMan.h"
#include "graphics/infoPopup.h"
#include "booru.h"
#include <gtkmm.h>
#include <memory>
#include <algorithm>


int main(int argc, char *const argv[])
{
	//dump args into a vector
	std::vector<std::string> args = [&](void)
	{ 
		std::vector<std::string> acc;

		for(int i = 0; i < argc; i++)
		{
			acc.push_back(std::string(argv[i]));
		}

		return acc;
	}();

	const auto scan = std::find(args.begin(), args.end(), "-scan");
	const auto gui  = std::find(args.begin(), args.end(), "-gui");
	const auto clean = std::find(args.begin(), args.end(), "-clean");
	const auto twitter = std::find(args.begin(), args.end(), "-twitter");

	if(scan == args.end() && gui == args.end() && clean == args.end() && twitter == args.end()) return -1;

	auto app = Gtk::Application::create();
	auto builder = Gtk::Builder::create_from_file("window.glade");

	cute::HashDB hashDb("files.csv");
	cute::ThumbDB thumbDb("thumbnails");
	
	cute::ComTags artists("data/artists.csv", "data/artistScores.csv");
	cute::ComTags characters("data/chars.csv", "data/charScores.csv");
	cute::ComTags booruTags("data/booru.csv", "data/booruScores.csv");

	cute::IdentityRank idRank;
	cute::PathRank path;


	if(twitter < args.end() - 1)
	{

		cute::CollectionMan collection ( idRank, path, booruTags, artists, characters);


		hashDb.scanDirectoryRecursive(*(twitter + 1));
		std::vector<cute::SharedImage> images  = collection.getImages();
		for(auto &f : std::filesystem::recursive_directory_iterator(*(twitter +1), std::filesystem::directory_options::follow_directory_symlink))
		{
			if(!cute::conformingFileType(f.path())) continue;
			images.push_back( std::make_shared<cute::Image> (f.path(), hashDb.retrieveData(f.path())));
		}
		collection.setImages(images);
		collection.filter();

		const auto cuteImage = collection.getGoodImage();
		const auto artist	 = artists.getTags(*cuteImage);
		const auto character = characters.getTags(*cuteImage);
		auto tags		 = collection.tagsWithScores(cuteImage);

		std::cout << cuteImage->location << std::endl;
		std::cout << "cutescore: " << idRank.getSkill(*cuteImage).skill() << std::endl;
		if(artist.size())
		{
			std::cout << "artist: ";
			for(const auto& t : artist) 
			{
				std::cout << t.tag << ' ';
			}
			std::cout << '\n';
		}
		if(tags.size())
		{
			std::sort(tags.begin(), tags.end(),
						[](const auto a, const auto b)
						{ return std::get<1>(a).skill() > std::get<1>(b).skill();});
			
			int count = 0;

			for(auto i = tags.begin(); i < tags.end(); i++)
			{
				if(count >= 3) break;

				std::cout << std::get<0>(*i).tag << ' ';
				count++;
			}
			std::cout << '\n';
		}


		return 0;
	}

	if(scan < args.end() - 1) 
	{
		cute::AsyncScanner scanner
		(*(scan +1), hashDb, thumbDb, booruTags.tags, characters.tags, artists.tags);

		scanner.scan();
		return 0;

	}

	if(clean < args.end() - 1) 
	{

		return cute::booruClean(*(clean + 1), hashDb, booruTags.tags, artists.tags, characters.tags);
	}

	if(gui == args.end()) return -1;


	cute::CollectionMan collection ( idRank, path, booruTags, artists, characters);
	graphics::BrowseWindow w(builder, collection, hashDb);

	return app->run(*w.getWindow());
}





