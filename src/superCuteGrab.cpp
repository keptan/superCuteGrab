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

	if(scan == args.end() && gui == args.end()) return -1;

	auto app = Gtk::Application::create();
	auto builder = Gtk::Builder::create_from_file("window.glade");

	cute::HashDB hashDb("files.csv");
	cute::ThumbDB thumbDb("thumbnails");
	
	cute::ComTags artists("data/artists.csv", "data/artistScores.csv");
	cute::ComTags characters("data/chars.csv", "data/charScores.csv");
	cute::ComTags booruTags("data/booru.csv", "data/booruScores.csv");

	cute::IdentityRank idRank;
	cute::PathRank path;

	if(scan < args.end() - 1) 
	{
		return cute::booruScan(*(scan + 1), hashDb, booruTags.tags, artists.tags, characters.tags);
	}

	if(gui == args.end()) return -1;


	cute::CollectionMan collection ( idRank, path, booruTags, artists, characters);
	graphics::BrowseWindow w(builder, collection, hashDb);

	return app->run(*w.getWindow());
}





