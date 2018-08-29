#include <iostream>
#include "hashDB.h"
#include "thumbDB.h"
#include "graphics/window.h"
#include <gtkmm.h>


int main(int argc, char *const argv[])
{
	auto app = Gtk::Application::create();
	cute::HashDB hashDb("files.csv");
	hashDb.scanDirectory("test");
	hashDb.writeCSV();

	cute::ThumbDB thumbDb("thumbnails");

	cute::Image image("test/anime.png", hashDb.retrieveData("test/anime.png"));

	std::cout << thumbDb.getThumbPath(image) << std::endl;

	auto builder = Gtk::Builder::create_from_file("window.glade");
	
	Window w(builder);
	//return app->run(*w.getWindow());
}





