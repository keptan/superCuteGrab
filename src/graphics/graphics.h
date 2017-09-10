#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <gtk/gtk.h>
#include <string>
#include <vector>
#include "../metaData.h"

namespace cute
{
	struct resizeData
	{
		GtkWidget *image;
		GdkPixbuf *sourcePixbuf;
		GtkWidget *aspect;
		GtkWidget *viewport;
		
	};


	gboolean resize_image(GtkWidget *widget, GtkAllocation *allocation, resizeData *data);	
	gboolean sizeChanged(GtkWidget *widget, GtkAllocation *allocation, resizeData *data);
	gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);
	int localGinit();

	class CompareWindow
	{
		private:
			static GtkWidget *image1;
			static GtkWidget *image2;

			cute::MetaData *input1;
			cute::MetaData *input2;


		GtkWidget *leftText;
		GObject *leftBuffer;

		GtkWidget *rightText;
		GObject *rightBuffer;



		GtkBuilder *builder;



		int runText();
		std::vector<resizeData*> viewData;


			//gboolean resize_imaeg(GtkWidget *widget, GdkEvent *event, GtkWindow *window);

			resizeData *newImageBox(std::string i,int s);

		public:
			CompareWindow();

			GtkWidget *newWindow(cute::MetaData *i1, cute::MetaData *i2);

	};

}	

#endif
