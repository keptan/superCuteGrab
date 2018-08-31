#include "window.h" 
#include <iostream> 
#include <random>
#include <cmath>

Window :: Window ( const Glib::RefPtr<Gtk::Builder> b, cute::CollectionMan& c)
	: builder(b), leftImage(builder, "leftAspect", "leftScrollView", "leftImage") 
				  , rightImage(builder, "rightAspect", "rightScrollView", "rightImage")
				  , collection(c), thumbnails("thumbnails"), lastSize(false)

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
		view->signal_item_activated().connect(sigc::mem_fun(*this, &Window::selected));
	}
	window->signal_key_press_event().connect(sigc::mem_fun(*this, &Window::onKeyPress), false);

	collection.freshImages();
	leftImage.setImage( collection.getLeftImage());
	rightImage.setImage( collection.getRightImage());

	for(const auto& i : collection.getImages())
		addMember(i);

	window->show_all_children();
}

void Window::on_button_quit (void)
{
	window->hide();
}

void Window :: selected (const Gtk::TreeModel::Path& path)
{
	auto iter = m_refTreeModel->get_iter(path);
	auto row = *iter; 

	std::shared_ptr<cute::Image>  image = row[m_Columns.m_col_image];

	collection.setLeftImage(image);

	leftImage.setImage( collection.getLeftImage());
	rightImage.setImage(collection.getRightImage());

	leftImage.scaleImage( leftImage.scrollView->get_allocation());
	rightImage.scaleImage( rightImage.scrollView->get_allocation());

	window->resize( window->get_width() + 1, window->get_height() + 1);
	window->resize( window->get_width() + 1, window->get_height() + 1);
	window->resize( window->get_width() - 1, window->get_height() - 1);
	window->resize( window->get_width() + 1, window->get_height() + 1);
	window->resize( window->get_width() - 1, window->get_height() - 1);
}



void Window :: addMember (const std::shared_ptr<cute::Image> i)
{
	Gtk::TreeModel::Row r = *(m_refTreeModel->append());

	r[m_Columns.m_col_name] = i->location.string();
	r[m_Columns.m_col_pixbuf] = Gdk::Pixbuf::create_from_file( thumbnails.getThumbPath(*i).c_str());
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

}

Window::ScalingImage :: ScalingImage (const Glib::RefPtr<Gtk::Builder> builder, 
		const std::string& a, const std::string& s, const std::string& i)
{
	builder->get_widget(a.c_str(), aspectFrame);
	builder->get_widget(s.c_str(), scrollView);
	builder->get_widget(i.c_str(), image);


	scrollView->signal_size_allocate().connect(sigc::mem_fun(*this, &Window::ScalingImage::scaleImage));
};

void Window::ScalingImage :: setImage (const std::shared_ptr<cute::Image> i)
{
	cuteImage = i;
	path = cuteImage->location;
	sourcePBuf = Gdk::Pixbuf::create_from_file(path);

	const int width = sourcePBuf->get_width();
	const int height = sourcePBuf->get_height(); 
	const float ratio = (float) width / (float) height; 

	aspectFrame->set(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER, ratio, false); 
	image->set(sourcePBuf); 
	auto a = scrollView->get_allocation();
	scaleImage(a);
}

void Window::ScalingImage :: scaleImage (Gtk::Allocation allocation)
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



bool Window :: onKeyPress (GdkEventKey *event)
{

	const auto spamRes = [&](void) 
	{ 
		leftImage.scaleImage( leftImage.scrollView->get_allocation());
		rightImage.scaleImage( rightImage.scrollView->get_allocation());


		if(lastSize)
		{
			window->resize( window->get_width() - 5, window->get_height() - 5);
			window->resize( window->get_width() + 1, window->get_height() + 1);

		//	leftImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&leftImage, &Window::ScalingImage::scaleImage));
		//	rightImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&rightImage, &Window::ScalingImage::scaleImage));


			window->resize( window->get_width() - 5, window->get_height() - 5);
			window->resize( window->get_width() + 5, window->get_height() + 5);
		}
		else 
		{

		//	leftImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&leftImage, &Window::ScalingImage::scaleImage));
		//	rightImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&rightImage, &Window::ScalingImage::scaleImage));

			window->resize( window->get_width() + 5, window->get_height() + 5);
			window->resize( window->get_width() - 5, window->get_height() - 5);
		}


		lastSize = !lastSize;
	};

	
	if( event->hardware_keycode == 113) 
	{
		//left key 
		collection.leftVictory(); 
		leftImage.setImage( collection.getLeftImage());
		rightImage.setImage( collection.getRightImage());
	
		spamRes();
	
		return true; 
	}

	if( event->hardware_keycode == 111)
	{
		//up key

		collection.tieVictory(); 
		leftImage.setImage( collection.getLeftImage());
		rightImage.setImage( collection.getRightImage());
		spamRes();
		return true;
	}


	if( event->hardware_keycode == 114) 
	{
	
		//right key 
		collection.rightVictory(); 
		leftImage.setImage( collection.getLeftImage());
		rightImage.setImage( collection.getRightImage());

		spamRes();
	
		return true; 
	}


	if( event->hardware_keycode == 116) 
	{
		//down key 
		spamRes();

		leftImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&leftImage, &Window::ScalingImage::scaleImage));
		rightImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&rightImage, &Window::ScalingImage::scaleImage));

		spamRes();
		return true;
	}

	std::cout << event->hardware_keycode << '\n';

	return false;





}
