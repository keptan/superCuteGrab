#include "imageIcons.h" 
#include <iostream> 

ImageIcons :: ImageIcons (void)
{
	auto item = Gtk::manage( new Gtk::MenuItem("_Info", true));
	item->signal_activate().connect(
			sigc::mem_fun(*this, &ImageIcons::info_item) );

	m_Menu_Popup.append(*item);

	//enable multiple selected
	set_selection_mode(Gtk::SELECTION_MULTIPLE);	
	/*
	set_row_spacing(0);
	set_column_spacing(0);
	set_item_padding(0);
	set_spacing(0);
	*/


	m_Menu_Popup.accelerate(*this);
	m_Menu_Popup.show_all();
}

ImageIcons :: ~ImageIcons (void)
{}

bool ImageIcons :: on_button_press_event (GdkEventButton* button_event)
{
	bool return_value = false; 

	return_value = IconView::on_button_press_event(button_event);

	if (( button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3) )
	{
		std::cout << button_event->button << std::endl;
		m_Menu_Popup.popup_at_pointer((GdkEvent*)button_event);

	}
	return return_value; 
}


void ImageIcons :: info_item (void)
{
	std::cout << "item selected " << std::endl;
	const auto ref = get_selected_items();

	m_signal_image.emit(ref);
}
	
ImageIcons::type_imageSig ImageIcons :: signal_image (void)
{
	return m_signal_image;
}



