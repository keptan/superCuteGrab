#include <gtk/gtk.h>
#include <string>
#include <vector>

#include "graphics.h"
#include "metaData.h"

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
		GdkPixbuf *sourcePixbuf = data->sourcePixbuf;
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
	
	int Window :: runText()
	{
		gtk_text_buffer_set_text(GTK_TEXT_BUFFER(leftBuffer),input1->tagsString().c_str(),input1->tagsString().length());
	}
		


	resizeData *Window :: newImageBox(std::string i,int s)
	{
		viewData.push_back(new resizeData);
		resizeData* image = viewData.back();


		if(s){
		image->image =  GTK_WIDGET(gtk_builder_get_object(builder,"image1"));
		image->viewport = GTK_WIDGET(gtk_builder_get_object(builder,"viewport1"));
		image->aspect =  GTK_WIDGET(gtk_builder_get_object(builder,"aspectframe1"));
		}
		else{	
		image->image =  GTK_WIDGET(gtk_builder_get_object(builder,"image2"));
		image->viewport = GTK_WIDGET(gtk_builder_get_object(builder,"viewport2"));
		image->aspect =  GTK_WIDGET(gtk_builder_get_object(builder,"aspectframe2"));
		}

		
		image->sourcePixbuf = gdk_pixbuf_new_from_file(i.c_str(),NULL);
		gtk_image_set_from_pixbuf(GTK_IMAGE(image->image),gdk_pixbuf_copy(image->sourcePixbuf));


		int width = gdk_pixbuf_get_width(image->sourcePixbuf);
		int height = gdk_pixbuf_get_height(image->sourcePixbuf);

		float ratio = width / (float) height;
	

		gtk_aspect_frame_set(GTK_ASPECT_FRAME(image->aspect),0,0,ratio,FALSE);


		g_signal_connect(image->viewport,"size-allocate",G_CALLBACK(sizeChanged),image);

		
		return image;

	}

	GtkWidget* Window :: newWindow(cute::MetaData *i1, cute::MetaData *i2)
	{
		GtkWidget *window;	
		GtkWidget *hbox;


		input1 = i1;
		input2 = i2;


		gtk_init(NULL,NULL);
		builder = gtk_builder_new();
		gtk_builder_add_from_file(builder, "window.glade", NULL);

		window = GTK_WIDGET(gtk_builder_get_object(builder,"window1"));
		hbox = GTK_WIDGET(gtk_builder_get_object(builder, "table1"));

		leftText= GTK_WIDGET(gtk_builder_get_object(builder,"textview2"));
		leftBuffer = G_OBJECT(gtk_text_buffer_new(NULL));
		gtk_text_view_set_buffer(GTK_TEXT_VIEW(leftText),GTK_TEXT_BUFFER(leftBuffer));

		rightText= GTK_WIDGET(gtk_builder_get_object(builder,"textview1"));
		rightBuffer = G_OBJECT(gtk_text_buffer_new(NULL));
		gtk_text_view_set_buffer(GTK_TEXT_VIEW(rightText),GTK_TEXT_BUFFER(rightBuffer));

		g_signal_connect(G_OBJECT (window), "delete_event", G_CALLBACK(delete_event),NULL);

		resizeData *image1 = newImageBox(input1->filePath(),0);
		resizeData *image2 = newImageBox(input2->filePath(),1);

		runText();

	//gtk_container_add(GTK_CONTAINER(hbox),image1->aspect);
//	gtk_container_add(GTK_CONTAINER(hbox),image2->aspect);
		//gtk_container_add(GTK_CONTAINER(window),hbox);

		/*
		gtk_window_set_default_size(GTK_WINDOW(window),
		gdk_pixbuf_get_width(image1->sourcePixbuf),
		gdk_pixbuf_get_height(image1->sourcePixbuf));
		*/

		gtk_widget_show_all(window);
		gtk_main();

		return window; 

	}

}
