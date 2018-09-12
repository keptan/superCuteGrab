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

class InfoPopup 
{


	//image databases ect 
	std::shared_ptr<cute::Image > image;
	cute::CollectionMan& collection;

	//widgets and gtkmm pointers 
	Gtk::Window* window; 
	Gtk::Label* scoreLabel;
	Gtk::TreeView* tagTree;
	Glib::RefPtr<Gtk::Builder> builder; 

	//scaling image struct 
	ScalingImage infoImage;

	public: 
	InfoPopup ( const Glib::RefPtr<Gtk::Builder>, cute::CollectionMan&, std::shared_ptr< cute::Image>);
	virtual ~InfoPopup (void);
	void setImage (std::shared_ptr< cute::Image>);

	Gtk::Window* getWindow (void);

	protected: 
	//handlers 



	class Columns : public Gtk::TreeModel::ColumnRecord 
	{
		public: 
		Columns (void)
		{
			add(m_col_name);
			//add(m_col_score);
		}

		Gtk::TreeModelColumn<Glib::ustring> m_col_name; 
	};

	Columns m_Columns;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};

		
class BrowseWindow : public sigc::trackable 
{

	FightWindow fight;
	private:
	const Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window* window; 
	ImageIcons view;
	Gtk::Menu menuPopup;

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

	void on_dropped_file(const Glib::RefPtr<Gdk::DragContext>& context,
	  int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);

	void get_selected_data (
		   const Glib::RefPtr<Gdk::DragContext>& context, 
			Gtk::SelectionData& selection_data, guint info, guint time);	   

	void callback (const std::vector<Gtk::TreeModel::Path>);


	class Columns : public Gtk::TreeModel::ColumnRecord 
	{
		public:
		Columns (void)
		{
			add(m_col_name);
			add(m_col_pixbuf);
			add(m_col_image);
		}


		Gtk::TreeModelColumn<Glib::ustring> m_col_name; 
		Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_col_pixbuf;
		Gtk::TreeModelColumn< std::shared_ptr<cute::Image>> m_col_image;
	};

	Columns m_Columns;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};


