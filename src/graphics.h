#include <gtk/gtk.h>
#include <string>
#include <vector>

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
	static gboolean sizeChanged(GtkWidget *widget, GtkAllocation *allocation, resizeData *data);
	gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

	class Window
	{
		public:
			static GtkWidget *image1;
			static GtkWidget *image2;

		GdkPixbuf *sourcePixbuf = NULL;

		std::vector<resizeData*> viewData;


			//gboolean resize_imaeg(GtkWidget *widget, GdkEvent *event, GtkWindow *window);

			resizeData *newImageBox(std::string i);
			GtkWidget *newWindow( std::string i1, std::string i2);

		public:
			Window();

	};

}	
