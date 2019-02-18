#pragma once 

#include <gtkmm.h> 
#include <memory>
#include <string>
#include "../image.h"
#include "../thumbDB.h"
#include "../compMan.h"
#include "../hashDB.h"
#include "../collectionMan.h"
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
	}


	Gtk::TreeModelColumn<Glib::ustring> m_col_name; 
	Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_col_pixbuf;
	Gtk::TreeModelColumn< std::shared_ptr<cute::Image>> m_col_image;
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
	std::vector< std::shared_ptr< cute::Image>> selected;

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

	public: 
	InfoPopup ( const Glib::RefPtr<Gtk::Builder>, cute::ThumbDB&, cute::CollectionMan&);
	virtual ~InfoPopup (void);

	void setImages (const std::vector< std::shared_ptr< cute::Image>>);
	Gtk::Window* getWindow (void);

	void insertTag (void);
	void insertCharacter (void);
	void insertArtist (void);

	protected: 
	//handlers 

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

		
class BrowseWindow : public sigc::trackable 
{

	FightWindow fight;
	private:
	const Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window* window; 
	ImageIcons view;

	Gtk::Menu menuPopup;
	Gtk::Entry* filterSearch;
	Gtk::TreeView* tagTree;

	std::unique_ptr<InfoPopup> iPop;
	cute::ThumbDB thumbnails; 
	cute::CollectionMan& collection; 
	cute::HashDB& hash;


	public:
	BrowseWindow( const Glib::RefPtr<Gtk::Builder>, cute::CollectionMan&, cute::HashDB&);
	Gtk::Window* getWindow (void);

	protected:
	void selected (const Gtk::TreeModel::Path&);
	void addMember (const std::shared_ptr<cute::Image> i);
	void import_folder (void);
	void import_folder_recursive (void);
	void filter (void);
	void refreshTagTree (void);
	void filterTagTree (void);
	void refresh (void);

	void on_dropped_file(const Glib::RefPtr<Gdk::DragContext>& context,
	  int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);

	void get_selected_data (
		   const Glib::RefPtr<Gdk::DragContext>& context, 
			Gtk::SelectionData& selection_data, guint info, guint time);	   

	void callback (const std::vector<Gtk::TreeModel::Path>);


	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	IconColumns m_Columns;

	Glib::RefPtr<Gtk::ListStore> tagTreeModel;
	TagColumns tagColumns;

};

}

