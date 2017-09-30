#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <gtkmm.h>
#include <string>
#include <vector>
#include "scalingImage.h"
#include "../metaData.h"
#include "../imageBase.h"
#include <mutex>
#include <thread>



namespace cute
{

	static std::mutex m;
	struct resizeData
	{
		Gtk::Image *image;
		Gtk::Viewport *view;
		Gtk::ScrolledWindow *scrollView;
		Gtk::AspectFrame *aspect;
		Glib::RefPtr<Gdk::Pixbuf> sourcePixbuf;
		MetaData *data;
	};
	
	class SearchWindow : public Gtk::Window
	{
		protected:

			std::vector<std::thread *> threads;

			Glib::RefPtr<Gtk::Builder> builder;

			Gtk::IconView *iconView;
			Gtk::Entry *entry;

			resizeData *image;
			resizeData *image2;

			MetaData *metaData;
			MetaData *metaData2;

			Gtk::ScrolledWindow *iconScroll;

			std::vector<MetaData> *population;
			cute::ImageBase *base;

			bool sized = true;
			bool busy = true;

		public:

			void baseInit(ImageBase *b);
			SearchWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder> &refGlade);
		protected:
			void populate(std::string t);
			void populate();
			void on_image_resize();
			void on_icon_activated();
			int leftCount = 0;
			int rightCount = 0;
			void entry_activated();

			bool addThumb(Gtk::TreeModel::Path path);

			void on_item_activated(const Gtk::TreeModel::Path &path);

			void add_entry(const std::string &filename,int loc);
			bool on_scroll(GdkEventScroll *e);
			resizeData *newImageBox(std::string i);

			void rightsizeChanged(Gtk::Allocation& allocation);

			resizeData *rightImageBox(std::string i);

			void sizeChanged(Gtk::Allocation& allocation);

			void mansizeChanged();

			void imageClicked();

			void sizeChangedII(Gtk::Allocation& allocation);

			void rightsizeChangedII(Gtk::Allocation& allocation);
			bool onKeyPress(GdkEventKey *event);



		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns()
				{
					add(m_col_name);
					add(m_col_pixbuf);
					add(m_col_path);
					add(m_col_data);
					add(m_col_hasThumb);
					add(m_col_thread);
				}

				Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_col_pixbuf;
				Gtk::TreeModelColumn<std::string> m_col_name;
				Gtk::TreeModelColumn<std::string> m_col_path;
				Gtk::TreeModelColumn<int> m_col_hasThumb;
				Gtk::TreeModelColumn<int> m_col_thread;
				Gtk::TreeModelColumn<int> m_col_data;
		};

		class TagModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				TagModelColumns()
				{
					add(m_col_name);
				}

				

				Gtk::TreeModelColumn<std::string> m_col_name;
		};

		ModelColumns m_Columns;
		TagModelColumns t_Columns;
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
			 


