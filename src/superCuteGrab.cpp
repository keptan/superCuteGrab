#include <iostream>
#include "hashDB.h"
#include "thumbDB.h"
#include "graphics/window.h"
#include <gtkmm.h>
#include <memory>


int main(int argc, char *const argv[])
{
	auto app = Gtk::Application::create();
	auto builder = Gtk::Builder::create_from_file("window.glade");
	Window w(builder);

	cute::HashDB hashDb("files.csv");
	cute::ThumbDB thumbDb("thumbnails");

	hashDb.scanDirectory("test");
	hashDb.writeCSV();


	std::vector< std::shared_ptr<cute::Image>> images;


	for(auto &f : std::filesystem::directory_iterator("test"))
	{
		if(!cute::conformingFileType(f.path())) continue;
		images.push_back( std::make_shared<cute::Image> (f.path(), hashDb.retrieveData(f.path())));
	}

	//sanitize incoming paths for webms and stuff
	for(auto &i : images)
	{	

		w.addMember(i, thumbDb.getThumbPath(*i).string());

	}





	return app->run(*w.getWindow());
}





