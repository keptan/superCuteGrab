#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <gtk/gtk.h>
#include <string>
#include <vector>
#include "../metaData.h"
#include "graphics.h"


namespace cute
{
	enum
	{
		COL_LOC,
		COL_NAME,
		COL_PIXBUF,
		NUM_COLS
	};


	class SearchWindow	
	{
		private :


			resizeData image;
			GtkBuilder *builder;
			GtkListStore *store;
			GtkTreeModel *model;
			GtkWidget *view;
			int  newImageBox(std::string i = "test.png" );

			GdkPixbuf *pixbuf;
			GtkTreeModel *create_and_fill_model();
			GtkWidget *create_view_and_model();

		public :
			SearchWindow();
			int createWindow();
			int populate(std::vector<MetaData>);
	};
};

#endif
			 


