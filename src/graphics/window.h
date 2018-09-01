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
	ScalingImage leftImage, rightImage;
	cute::CollectionMan& collection;
	bool lastSize;

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



