#pragma once 

#include <gtkmm.h> 
#include <memory>
#include <string>
#include "../image.h"
#include "../thumbDB.h"
#include "../compMan.h"
#include "../collectionMan.h"

class Window 
{
	struct ScalingImage
	{
		std::shared_ptr<cute::Image> cuteImage; 

		std::string path;
		Glib::RefPtr<Gdk::Pixbuf> sourcePBuf; 

		//gonna be annoying and tricky to construct this 
		Gtk::ScrolledWindow* scrollView; 
		Gtk::AspectFrame* aspectFrame; 
		Gtk::Image* image; 

		ScalingImage (Gtk::ScrolledWindow*, Gtk::AspectFrame*, Gtk::Image*);
		ScalingImage (const Glib::RefPtr<Gtk::Builder> builder, 
		const std::string& aspect, const std::string& scroll, const std::string& image);

		ScalingImage (void){};

		void setImage	(std::string);
		void setImage (const std::shared_ptr<cute::Image> i);
		void scaleImage (Gtk::Allocation);
	};
		
	private:
	const Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window* window;
	ScalingImage leftImage, rightImage;

	cute::CollectionMan& collection;
	cute::ThumbDB thumbnails;
	bool lastSize;

	public:

	Window (const Glib::RefPtr<Gtk::Builder>, cute::CollectionMan&); 
	void addMember (const std::shared_ptr<cute::Image> i);

	Gtk::Window* getWindow (void);

	protected: 
	//signal handlers 
	void on_button_quit (void); 
	void selected (const Gtk::TreeModel::Path& path);
	bool onKeyPress (GdkEventKey *event);
	
	class Columns : public Gtk::TreeModel::ColumnRecord 
	{
		public:
		Columns  (void)
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


