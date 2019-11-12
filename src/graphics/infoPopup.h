#pragma once 

#include <gtkmm.h> 
#include <memory>
#include <string>
#include "../image.h"
#include "../thumbDB.h"
#include "../compMan.h"
#include "../hashDB.h"
#include "../collectionMan.h"
#include "../../lib/nfun/future_pool.h"
#include "window.h"
#include <sigc++/sigc++.h>
#include "scalingImage.h"
#include "imageIcons.h"

#include <iostream>

namespace graphics
{

class IconColumns : public Gtk::TreeModel::ColumnRecord 
{
	public:
	IconColumns (void)
	{
		add(m_col_name);
		add(m_col_pixbuf);
		add(m_col_image);
		add(m_col_hasPixbuf);
	}


	Gtk::TreeModelColumn<Glib::ustring> m_col_name; 
	Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_col_pixbuf;
	Gtk::TreeModelColumn< std::shared_ptr<cute::Image>> m_col_image;
	Gtk::TreeModelColumn<bool> m_col_hasPixbuf;
};

class TagColumns : public Gtk::TreeModel::ColumnRecord 
{
	public: 
	TagColumns (void)
	{
		add(m_col_name);
		add(m_col_score);
	}

	Gtk::TreeModelColumn<Glib::ustring> m_col_name; 
	Gtk::TreeModelColumn<int> m_col_score;
};





class InfoPopup 
{

	//thumbnail DB, and the collection manager
	//that we need to actually append tags 
	cute::ThumbDB& thumbnails;
	cute::CollectionMan& collection;
	
	
	//builder that contains the widgets we'll use 
	Glib::RefPtr<Gtk::Builder> builder; 

	//image databases ect 
	std::vector< cute::SharedImage> selected;

	//widgets and gtkmm pointers 
	Gtk::Window* window; 
	Gtk::TreeView* tagTree;
	Gtk::Entry* addTag;
	Gtk::Entry*	character;
	Gtk::Entry* artist;
	Gtk::Entry*	viewCharacter;
	Gtk::Entry* viewArtist;
	Gtk::AspectFrame* frame;
	Gtk::ScrolledWindow* scroll;


	//scaling image struct 
	ScalingImage infoImage;

	//icon viewer
	ImageIcons icons;
	bool lastSize;


	public: 
	InfoPopup ( const Glib::RefPtr<Gtk::Builder>, cute::ThumbDB&, cute::CollectionMan&);
	virtual ~InfoPopup (void);

	void setImages (const std::vector< std::shared_ptr< cute::Image>>);
	void setImages (const cute::SharedImage);
	Gtk::Window* getWindow (void);

	void insertTag (void);
	void insertCharacter (void);
	void insertArtist (void);

	protected: 
	//handlers 
	//
	bool onKeyPress (GdkEventKey *event);
	void upScroll (void);
	void downScroll (void);
	void scale_in_hell (void);


	Glib::RefPtr<Gtk::ListStore> tagTreeModel;
	TagColumns tagColumns;

	class CompleteCol : public Gtk::TreeModel::ColumnRecord
	{
		public:
		CompleteCol (void)
		{
			add(m_col_name);
			add(m_col_id);
		}
		Gtk::TreeModelColumn<Glib::ustring> m_col_name; 
		Gtk::TreeModelColumn<int> m_col_id;
	};

	CompleteCol c_Columns;
	std::map<int, Glib::ustring> m_CompleteActions;

	Glib::RefPtr<Gtk::ListStore> iconTreeModel;
	IconColumns iconColumns;


};

		


}

