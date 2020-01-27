#pragma once 

#include <gtkmm.h> 
#include <memory>
#include <string>
#include "../image.h"
#include "../thumbDB.h"
#include "../compMan.h"
#include "../collectionMan.h"
#include "scalingImage.h"



class FightWindow 
{

		
	private:
	const Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window* window;
	Gtk::Image* leftImage;
	Gtk::Image* rightImage; 
	cute::CollectionMan& collection;
	bool lastSize;

	void setImage (const std::shared_ptr<cute::Image> i, Gtk::Image*);

	public:
	FightWindow (const Glib::RefPtr<Gtk::Builder>, cute::CollectionMan&); 
	Gtk::Window* getWindow (void);
	void refresh (void);

	protected: 
	//signal handlers 
	void on_button_quit (void); 
	void selected (const Gtk::TreeModel::Path& path);
	bool onKeyPress (GdkEventKey *event);
};



