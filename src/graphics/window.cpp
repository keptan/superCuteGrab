#include "window.h" 
#include <iostream> 
#include <random>
#include <cmath>


FightWindow :: FightWindow ( const Glib::RefPtr<Gtk::Builder> b, cute::CollectionMan& c)
	: builder(b), leftImage(builder, "leftAspect", "leftScrollView", "leftImage") 
				  , rightImage(builder, "rightAspect", "rightScrollView", "rightImage")
				  , collection(c), lastSize(false)

{
	builder->get_widget("fightWindow", window);
	Gtk::IconView* view;
	window->signal_key_press_event().connect(sigc::mem_fun(*this, &FightWindow::onKeyPress), false);

	/*
	leftImage.setImage( collection.getLeftImage());
	rightImage.setImage( collection.getRightImage());
	*/

}

void FightWindow::on_button_quit (void)
{
	window->hide();
}

void FightWindow::refresh (void)
{
	leftImage.setImage( collection.getLeftImage());
	rightImage.setImage( collection.getRightImage());
}

Gtk::Window* FightWindow :: getWindow (void)
{
	return window;
}


bool FightWindow :: onKeyPress (GdkEventKey *event)
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


		spamRes();
		return true;
	}

	std::cout << event->hardware_keycode << '\n';

	return false;





}
