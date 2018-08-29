#include <iostream>
#include "hashDB.h"
#include "graphics/window.h"
#include <gtkmm.h>


int main(int argc, char *const argv[])
{
//	booruWriteScan(argv[1]);
	
	cute::HashDB db("files.csv");
	db.scanDirectory("test");
	db.writeCSV();

	auto app = Gtk::Application::create();
	auto builder = Gtk::Builder::create_from_file("window.glade");
	
	Window w(builder);
	return app->run(*w.getWindow());
}





