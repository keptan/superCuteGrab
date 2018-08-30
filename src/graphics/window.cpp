#include "window.h" 
#include <iostream> 

Window :: Window ( const Glib::RefPtr<Gtk::Builder> b)
	: builder(b), leftImage(builder, "leftAspect", "leftScrollView", "leftImage"), 
				  rightImage(builder, "rightAspect", "rightScrollView", "rightImage")
{
	m_refTreeModel = Gtk::ListStore::create(m_Columns); 

	builder->get_widget("SearchWindow", window);

	Gtk::IconView* view;
	builder->get_widget("iconView", view);
	if(view)
	{
		view->set_model(m_refTreeModel);
		view->set_pixbuf_column(m_Columns.m_col_pixbuf);
		view->set_markup_column(m_Columns.m_col_name);
	}

	leftImage.setImage("anime.png");
	rightImage.setImage("anime.png");

	window->show_all_children();
}

Window :: ~Window (void)
{}

void Window::on_button_quit (void)
{
	window->hide();
}

void Window :: addMember ( const std::string& name, const std::optional<std::string>& icon, const int id)
{
	Gtk::TreeModel::Row r = *(m_refTreeModel->append());
	r[m_Columns.m_col_name] = name.c_str(); 
	r[m_Columns.m_col_pixbuf] = Gdk::Pixbuf::create_from_file( icon.value_or("icon.png"));
}

void Window :: addMember (const std::shared_ptr<cute::Image> i, const std::string& t)
{
	Gtk::TreeModel::Row r = *(m_refTreeModel->append());

	r[m_Columns.m_col_name] = i->location.string();
	r[m_Columns.m_col_pixbuf] = Gdk::Pixbuf::create_from_file( t.c_str());
	r[m_Columns.m_col_image] = i;
}	


Gtk::Window* Window :: getWindow (void)
{
	return window;
}

Window::ScalingImage :: ScalingImage (Gtk::ScrolledWindow* w, Gtk::AspectFrame* a, Gtk::Image* i)
	: scrollView(w), aspectFrame(a), image(i)
{
	scrollView->signal_size_allocate().connect(sigc::mem_fun(*this, &Window::ScalingImage::scaleImage));
	setImage("anime.png");

}

Window::ScalingImage :: ScalingImage (const Glib::RefPtr<Gtk::Builder> builder, 
		const std::string& a, const std::string& s, const std::string& i)
{
	builder->get_widget(a.c_str(), aspectFrame);
	builder->get_widget(s.c_str(), scrollView);
	builder->get_widget(i.c_str(), image);


	scrollView->signal_size_allocate().connect(sigc::mem_fun(*this, &Window::ScalingImage::scaleImage));
	setImage("anime.png");
};

void Window::ScalingImage :: setImage (std::string p)
{
	path = p;
	sourcePBuf = Gdk::Pixbuf::create_from_file(path);

	const int width = sourcePBuf->get_width();
	const int height = sourcePBuf->get_height(); 
	const float ratio = (float) width / (float) height; 

	aspectFrame->set(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER, ratio, false); 
	image->set(sourcePBuf); 
	auto a = scrollView->get_allocation();
	scaleImage(a);
}

void Window::ScalingImage :: scaleImage (Gtk::Allocation& allocation)
{
	if( allocation.get_width() != image->get_width() ||
		allocation.get_height() != image->get_height())
	{
		image->set( sourcePBuf->scale_simple(
					allocation.get_width(), 
					allocation.get_height(),
					Gdk::INTERP_BILINEAR));
	}

	return; 
}
