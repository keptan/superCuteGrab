#include "window.h" 
#include "gdkmm/pixbuf.h"
#include <iostream> 
#include <random>
#include <cmath>


FightWindow :: FightWindow ( const Glib::RefPtr<Gtk::Builder> b, cute::CollectionMan& c)
	: builder(b), collection(c), lastSize(false)
{
	builder->get_widget("fightWindow", window);
	Gtk::IconView* view;
	window->signal_key_press_event().connect(sigc::mem_fun(*this, &FightWindow::onKeyPress), false);

	builder->get_widget("leftImage",leftImage);
	builder->get_widget("rightImage", rightImage);

	/*
	leftImage.setImage( collection.getLeftImage());
	rightImage.setImage( collection.getRightImage());
	*/
}


void FightWindow :: setImage (const std::shared_ptr<cute::Image> i, Gtk::Image* box)
{
	const auto source = Gdk::Pixbuf::create_from_file( i->location);

	const float width  = source->get_width();
	const float height = source->get_height();

	const float bWidth  = box->get_allocated_width();
	const float bHeight = box->get_allocated_height();

	const float ratio  = (float) height / width; 
	const float bRatio = bHeight / bWidth;

	if(bRatio > ratio)
	{
		box->set( source->scale_simple(
					width * (bWidth / width), height * (bWidth / width), Gdk::INTERP_BILINEAR));
		return;
	}

		box->set( source->scale_simple(
					width * (bHeight / height), height * (bHeight / height), Gdk::INTERP_BILINEAR));
}


	

void FightWindow::on_button_quit (void)
{
	window->hide();
}

void FightWindow::refresh (void)
{
	setImage(collection.getLeftImage(), leftImage);
	setImage(collection.getRightImage(), rightImage);
}

Gtk::Window* FightWindow :: getWindow (void)
{
	return window;
}


bool FightWindow :: onKeyPress (GdkEventKey *event)
{

	const auto spamRes = [&](void) 
	{ 
		//leftImage.scaleImage( leftImage.scrollView->get_allocation());
		//rightImage.scaleImage( rightImage.scrollView->get_allocation());

		/*

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
		*/


	};

	
	if( event->hardware_keycode == 113) 
	{
		//left key 
		collection.leftVictory(); 
		refresh();
	
		return true; 
	}

	if( event->hardware_keycode == 111)
	{
		//up key

		collection.tieVictory(); 
		refresh();
		return true;
	}


	if( event->hardware_keycode == 114) 
	{
	
		//right key 
		collection.rightVictory(); 
		refresh();
		return true; 
	}


	if( event->hardware_keycode == 116) 
	{
		//down key 
		refresh();
		return true;
	}

	std::cout << event->hardware_keycode << '\n';

	return false;





}
