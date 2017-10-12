#ifndef SCALINGIMAGE
#define SCALINGIMAGE
struct ScalingImage : public Gtk::Image
{
  explicit
  ScalingImage(Glib::RefPtr<Gdk::Pixbuf> pixbuf,
               Gdk::InterpType interp = Gdk::INTERP_BILINEAR):
    Gtk::Image(pixbuf),
    m_original(pixbuf),
    m_interp(interp),
    m_sized(false)
  {
  }

protected:
  virtual
  void
  on_size_allocate(Gtk::Allocation & r)
  {
    // This event is fired on all Widgets when their rectangle gets
    // resized. So we rescale our image. But rescaling our image fires
    // another resize event, and another, and another... You see the
    // problem!
    //
    // The most straightforward solution is to not rescale the image
    // the second time the event is fired. Hence the m_size toggle.
    //
    // Ideally I'd not like to do this but this was the best way I
    // found!
    if(!m_sized)
    {
      Glib::RefPtr<Gdk::Pixbuf> scaled = 
        m_original->scale_simple(r.get_width(),
                                 r.get_height(),
                                 m_interp);
      // Reset the image to be the new scaled image which
      // will cause the second size-allocate signal
      Gtk::Image::set(scaled);

      // Make sure we don't treat it as a rescale request 
      // next time
      m_sized = true;
    }
    else
    {
      // Reaction to set above. Call the base class's on_size_allocate
      // function. Shouldn't I have to call this above too? Maybe it
      // isn't necessary since it gets called anyway.
      Gtk::Image::on_size_allocate(r);

      // Treat next resize event as a rescale
      m_sized = false;
    }
  }
  
private:
  Glib::RefPtr<Gdk::Pixbuf> m_original;
  Gdk::InterpType m_interp;
  bool m_sized;
};

/// This widget automatically scales the image but preserves the
/// aspect ratio of the original image. This is accomplished via use
/// of the AspectFrame class
///
/// Ideally would like this to be a Gtk::Image subclass.
struct AspectPreservingScalingImage : public Gtk::AspectFrame
{
  explicit
  AspectPreservingScalingImage(Glib::RefPtr<Gdk::Pixbuf> const & pixbuf,
                               Gdk::InterpType interp = Gdk::INTERP_BILINEAR):
    m_img(pixbuf,interp)
  {
    set(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,
        // Aspect ratio of frame
        pixbuf->get_width()/float(pixbuf->get_height()));
    // This makes it appear as if there is no frame
    set_shadow_type(Gtk::SHADOW_NONE);
    // This allows a minimum size
    set_size_request(pixbuf->get_width()/2,pixbuf->get_height()/2);
    // Finally, add the image to be managed
    add(m_img);
  }
private:
  ScalingImage m_img;  
};

#endif
