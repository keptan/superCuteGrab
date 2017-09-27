#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <gtkmm.h>
#include <string>
#include <vector>
#include "../metaData.h"
#include "../imageBase.h"


namespace cute
{

	class SearchWindow : public Gtk::Window
	{
		protected:
			Glib::RefPtr<Gtk::Builder> builder;

			Gtk::IconView *iconView;
			Gtk::Image *image;
			Gtk::Entry *entry;

			std::vector<MetaData> *population;
			cute::ImageBase *base;

		public:
			SearchWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
		protected:
			void on_image_resize();
			void on_icon_activated();
			void on_entry_activated();
			void add_entry(const std::string &filename,int t);


		class TestData : public Glib::Object
		{
			public:
			
				int number;

				TestData(int t)
					: Glib::Object()
					, number(t)
				{}



				void operator = (int t){
				number = t;
				}
		};


		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns()
				{
					add(m_col_name);
					add(m_col_pixbuf);
				//	add(m_col_data);
				}
				int test;

				

				Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_col_pixbuf;
				//Gtk::TreeModelColumn<std::unique_ptr<TestData>> m_col_data;
				Gtk::TreeModelColumn<std::string> m_col_name;
		};

		ModelColumns m_Columns;
		Glib::RefPtr<Gtk::ListStore> refListModel;

	};
}


/*
	class SearchWindow	
	{
		private :


			GtkBuilder *builder;
			GtkWidget *entry;
			std::vector<MetaData> *population;
			cute::ImageBase *base;
			
			GtkWidget *scroll;

			GdkPixbuf *pixbuf;
			GtkWidget *create_view_and_model();

		public :


			resizeData image;
			resizeData image2;

			GtkWidget *view;
			GtkTreeModel *model;
			GtkListStore *store;
			SearchWindow(std::vector<MetaData> *p);
			SearchWindow(cute::ImageBase *b);
			int createWindow();

			int openImage(std::string p,int i = 1);
			GtkTreeModel *populate();
			GtkTreeModel *populate(std::string t);
	};

	void item_activated(GtkIconView *v,GtkTreePath *p,SearchWindow *user_data);
	void entry_activated(GtkWidget *w,SearchWindow *user_data);
	void scrolled(GtkWidget *w,SearchWindow *user_data);
};

*/
#endif
			 


