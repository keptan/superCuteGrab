#include <gtk/gtk.h>
#include <string>

namespace cute
{
	struct resizeData
	{
		GtkWidget *image;
		GdkPixbuf *pixbuf;
	};

	gboolean resize_image(GtkWidget *widget, GtkAllocation *allocation, resizeData *data);	
	static gboolean sizeChanged(GtkWidget *widget, GtkAllocation *allocation, resizeData *data);
	gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

	class Window
	{
		public:
			static GtkWidget *image1;
			static GtkWidget *image2;
	GtkWidget *viewport;
		GtkWidget *aspect;
		GdkPixbuf *sourcePixbuf = NULL;
		GtkWidget *image;

		resizeData viewData;


			//gboolean resize_imaeg(GtkWidget *widget, GdkEvent *event, GtkWindow *window);

			GtkWidget *newImageBox(std::string i);
			GtkWidget *newWindow( std::string i1, std::string i2);

		public:
			Window();

	};

}	
