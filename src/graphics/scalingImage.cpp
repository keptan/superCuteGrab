#include "scalingImage.h"

ScalingImage :: ScalingImage (Gtk::ScrolledWindow* w, Gtk::AspectFrame* a, Gtk::Image* i)
	: scrollView(w), aspectFrame(a), image(i)
{
	scrollView->signal_size_allocate().connect(sigc::mem_fun(*this, &ScalingImage::scaleImage));

}

ScalingImage :: ScalingImage (const Glib::RefPtr<Gtk::Builder> builder, 
		const std::string& a, const std::string& s, const std::string& i)
{
	builder->get_widget(a.c_str(), aspectFrame);
	builder->get_widget(s.c_str(), scrollView);
	builder->get_widget(i.c_str(), image);


	scrollView->signal_size_allocate().connect(sigc::mem_fun(*this, &ScalingImage::scaleImage));
};

void ScalingImage :: setImage (const std::shared_ptr<cute::Image> i)
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

void ScalingImage :: scaleImage (Gtk::Allocation allocation)
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


