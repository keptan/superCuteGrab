#include <gtk/gtk.h>
#include <string>
#include <vector>
#include "../metaData.h"
#include "searchWindow.h"

#include "graphics.h"


namespace cute
{
	SearchWindow :: SearchWindow()
	{
		createWindow();
	}

	GtkTreeModel *SearchWindow :: create_and_fill_model()
	{
		GtkTreeIter iter;

		pixbuf = gdk_pixbuf_new_from_file("icon.png",NULL);
		store = gtk_list_store_new(NUM_COLS,G_TYPE_STRING,G_TYPE_STRING,GDK_TYPE_PIXBUF);

		gtk_list_store_append(store,&iter);
		gtk_list_store_set(store,&iter,
							COL_LOC,"test.png",
							COL_NAME,"cute image",
							COL_PIXBUF,pixbuf,
							-1);

		gtk_list_store_append(store,&iter);
		gtk_list_store_set(store,&iter,
							COL_LOC,"test.png",
							COL_NAME,"cute image",
							COL_PIXBUF,pixbuf,
							-1);

		gtk_list_store_append(store,&iter);
		gtk_list_store_set(store,&iter,
							COL_LOC,"test.png",
							COL_NAME,"cute image",
							COL_PIXBUF,pixbuf,
							-1);


		return GTK_TREE_MODEL (store);
	}

	GtkWidget  *SearchWindow :: create_view_and_model()
	{
		model = create_and_fill_model();

		gtk_icon_view_set_model(GTK_ICON_VIEW(view),model);
		gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(view),GTK_SELECTION_SINGLE);
		gtk_icon_view_set_text_column(GTK_ICON_VIEW(view),COL_NAME);
		gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(view),COL_PIXBUF);

		return view;
	}

	int SearchWindow :: newImageBox(std::string i)
	{


		image.image =  GTK_WIDGET(gtk_builder_get_object(builder,"image5"));
		image.viewport = GTK_WIDGET(gtk_builder_get_object(builder,"viewport5"));
		image.aspect =  GTK_WIDGET(gtk_builder_get_object(builder,"aspectframe5"));


		
		image.sourcePixbuf = gdk_pixbuf_new_from_file(i.c_str(),NULL);
		gtk_image_set_from_pixbuf(GTK_IMAGE(image.image),gdk_pixbuf_copy(image.sourcePixbuf));


		int width = gdk_pixbuf_get_width(image.sourcePixbuf);
		int height = gdk_pixbuf_get_height(image.sourcePixbuf);

		float ratio = width / (float) height;
	

		gtk_aspect_frame_set(GTK_ASPECT_FRAME(image.aspect),0,0,ratio,FALSE);


		g_signal_connect(image.viewport,"size-allocate",G_CALLBACK(sizeChanged),&image);

		
		return 0;

	}


	int SearchWindow :: createWindow()
	{
		GtkWidget *window;


		builder = gtk_builder_new();
		gtk_builder_add_from_file(builder,"window.glade",NULL);

		window = GTK_WIDGET(gtk_builder_get_object(builder,"window3"));
		view = GTK_WIDGET(gtk_builder_get_object(builder,"iconview1"));
		newImageBox();

		create_view_and_model();

		gtk_widget_show_all(window);
		gtk_main();

		return 1;
	}
		

}
	
