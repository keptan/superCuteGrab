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
			GtkTreeModel *model;
			std::vector<MetaData> *population;
			
			GtkWidget *view;

			GdkPixbuf *pixbuf;
			GtkWidget *create_view_and_model();

		public :

			GtkListStore *store;
			SearchWindow(std::vector<MetaData> *p);
			int createWindow();

			int openImage(std::string p);
			GtkTreeModel *populate();
	};

	void item_activated(GtkIconView *v,GtkTreePath *p,SearchWindow *user_data);
};

#endif
			 


