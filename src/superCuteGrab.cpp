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
#include <gtkmm.h>
#include <memory>


int main(int argc, char *const argv[])
{
	auto app = Gtk::Application::create();
	auto builder = Gtk::Builder::create_from_file("window.glade");


	cute::HashDB hashDb("files.csv");
	cute::ThumbDB thumbDb("thumbnails");
	cute::TagDB tagDB("tags.csv");
	cute::IdentityRank idRank;

	hashDb.scanDirectory("test");
	hashDb.writeCSV();

	std::vector< std::shared_ptr<cute::Image>> images;


	for(auto &f : std::filesystem::directory_iterator("test"))
	{
		if(!cute::conformingFileType(f.path())) continue;
		images.push_back( std::make_shared<cute::Image> (f.path(), hashDb.retrieveData(f.path())));
	}

	cute::CollectionMan collection ( idRank, images);

	Window w(builder, collection);

	return app->run(*w.getWindow());

}





