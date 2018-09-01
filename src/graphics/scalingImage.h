#pragma once
#include <gtkmm.h> 
#include <memory>
#include <string>
#include "../image.h"

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


