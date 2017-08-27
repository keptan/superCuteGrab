#include <gtk/gtk.h>
#include <string>

#include "graphics.h"

namespace cute
{


	gboolean  resize_image(GtkWidget *widget, GdkEvent *event, GtkWindow *window)
	{
		GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(widget));
		
		pixbuf = gdk_pixbuf_scale_simple(pixbuf,widget->allocation.width,widget->allocation.height, GDK_INTERP_BILINEAR);

		gtk_image_set_from_pixbuf(GTK_IMAGE(widget),pixbuf);

		return FALSE;
	}

	static gboolean sizeChanged(GtkWidget *widget, GtkAllocation *allocation, resizeData *data)
	{
		GdkPixbuf *sourcePixbuf = data->pixbuf;
		GdkPixbuf *imagePixbuf;

		imagePixbuf = gtk_image_get_pixbuf(GTK_IMAGE(data->image));
		
		if (allocation->width != gdk_pixbuf_get_width(imagePixbuf)||
				allocation->height != gdk_pixbuf_get_height(imagePixbuf)){
			gtk_image_set_from_pixbuf(
					GTK_IMAGE(data->image),
					gdk_pixbuf_scale_simple(sourcePixbuf,
						allocation->width,
						allocation->height,
						GDK_INTERP_BILINEAR)
			);
				g_object_unref(imagePixbuf);
		}
		return FALSE;
	}

	gint  delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
	{
		gtk_main_quit();
		return false;
	}


	Window :: Window(){}	
	


	GtkWidget* Window :: newImageBox(std::string i)
	{
	
		sourcePixbuf = gdk_pixbuf_new_from_file(i.c_str(),NULL);
		image = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(sourcePixbuf));
		aspect = gtk_aspect_frame_new(NULL,0,0,1,TRUE);
		viewport = gtk_scrolled_window_new(NULL,NULL);

		gtk_container_add(GTK_CONTAINER(viewport),image);
		gtk_container_add(GTK_CONTAINER(aspect),viewport);

		g_signal_connect(viewport,"size-allocate",G_CALLBACK(sizeChanged),&viewData);

		viewData.image = image;
		viewData.pixbuf = sourcePixbuf;
		
		return aspect;

	}

	GtkWidget* Window :: newWindow(std::string i1, std::string i2)
	{
		GtkWidget *window;	
		GtkWidget *hbox;

		gtk_init(NULL,NULL);
		
		hbox = gtk_hbox_new(FALSE,0);
		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		g_signal_connect(G_OBJECT (window), "delete_event", G_CALLBACK(delete_event),NULL);


		gtk_container_add(GTK_CONTAINER(hbox),newImageBox(i1));
		gtk_container_add(GTK_CONTAINER(hbox),newImageBox(i2));
		gtk_container_add(GTK_CONTAINER(window),hbox);

		gtk_widget_show_all(window);
		gtk_main();

		return window; 

	}

}
