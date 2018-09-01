#pragma once 

#include <gtkmm.h>
#include <sigc++/sigc++.h>

class ImageIcons : public Gtk::IconView 
{
	public:
		typedef sigc::signal< void, std::vector<Gtk::TreeModel::Path>> type_imageSig;
		type_imageSig signal_image (void); 

		ImageIcons(void);
		virtual ~ImageIcons ();

	protected:
		type_imageSig m_signal_image;
		bool on_button_press_event (GdkEventButton* button_event) override; 

		void info_item (void);

		Gtk::Menu m_Menu_Popup;
};
