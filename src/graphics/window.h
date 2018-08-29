#pragma once 

#include <gtkmm.h> 
#include <string>

class Window 
{
	struct ScalingImage
	{
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
		void scaleImage (Gtk::Allocation&);
	};
		
	private:
	const Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window* window;
	ScalingImage leftImage, rightImage;


	public:
	Window (const Glib::RefPtr<Gtk::Builder>); 
	virtual ~Window (void); 
	void addMember (const std::string&, const std::optional<std::string>& , const int);
	Gtk::Window* getWindow (void);

	protected: 
	//signal handlers 
	void on_button_quit (void); 
	
	class Columns : public Gtk::TreeModel::ColumnRecord 
	{
		public:
		Columns  (void)
		{
			add(m_col_id); 
			add(m_col_name); 
			add(m_col_pixbuf);
		}

		Gtk::TreeModelColumn<unsigned int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name; 
		Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_col_pixbuf;
	};

	Columns m_Columns; 
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel; 
};


