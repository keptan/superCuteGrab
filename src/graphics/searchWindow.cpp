#include <gtkmm.h>
#include <string>
#include <vector>
#include <iostream>
#include "../metaData.h"
#include "../imageBase.h"
#include "searchWindow.h"



namespace cute
{
	SearchWindow :: SearchWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
		:Gtk::Window(cobject)
		,builder(refGlade)
	{
		builder->get_widget("iconview1",iconView);
		builder->get_widget("image5",image);
		builder->get_widget("entry1",entry);
		refListModel = Gtk::ListStore::create(m_Columns);




	}
	void SearchWindow :: baseInit(ImageBase *b)
	{

		base = b;
		base->readDirectory();
		base->filter("");
		population = base->collectImages();

		std::cout<<population->size()<<" "<<(*population)[population->size()-1].fileName()<<'\n';

		iconView->set_model(refListModel);
		iconView->set_markup_column(m_Columns.m_col_name);
		iconView->set_pixbuf_column(m_Columns.m_col_pixbuf);
		iconView->signal_item_activated().connect(sigc::mem_fun(*this,&SearchWindow::on_item_activated));
		entry->signal_activate().connect(sigc::mem_fun(*this,&SearchWindow::entry_activated));


	}

	void SearchWindow :: add_entry(const std::string &filename, int loc)
	{

		MetaData data = (*population)[loc];

		auto row =*(refListModel->append());
		row[m_Columns.m_col_name] = filename;
		row[m_Columns.m_col_path] = filename;
		row[m_Columns.m_col_data] = loc;
		row[m_Columns.m_col_pixbuf] = Gdk::Pixbuf::create_from_file(filename);

	}


	void SearchWindow :: on_item_activated(const Gtk::TreeModel::Path &path)
	{
		auto iter = refListModel->get_iter(path);
		auto row = *iter;

		int i = row[m_Columns.m_col_data];
		const std::string loc = row[m_Columns.m_col_path];
		const std::string name = row[m_Columns.m_col_path];

		MetaData localData = (*population)[i];

		std::cout<<localData.fileName()<<'\n';

	}

	void SearchWindow :: entry_activated()
	{
		std::string data = entry->get_text();
		populate(data);
		return;
	}
	void SearchWindow :: populate(std::string t)
	{
		base->filter(t);
		population = base->collectImages();

		refListModel = Gtk::ListStore::create(m_Columns);
		iconView->set_model(refListModel);
		iconView->set_markup_column(m_Columns.m_col_name);
		iconView->set_pixbuf_column(m_Columns.m_col_pixbuf);

		int i = 0;
		while(i < population->size())
		{
			MetaData localData = (*population)[i];
			std::cout<<localData.fileName()<<'\n';
			add_entry("test.png",i);
			++i;
		}
	

		//add_entry("test.png",1);
	}

};
/*

void entry_activated(GtkWidget *w,SearchWindow *user_data)
{
		const gchar *entry_text;
		entry_text = gtk_entry_get_text(GTK_ENTRY(w));

		user_data->populate(std::string(entry_text));
}

		

	SearchWindow :: SearchWindow(std::vector<MetaData> *p)
		:population(p)
	{
		createWindow();
	}
	SearchWindow :: SearchWindow(cute::ImageBase *b)
		:base(b)
	{
		b->readDirectory();
		population = b->collectImages();
		createWindow();
	}


	GtkWidget  *SearchWindow :: create_view_and_model()
	{
		model = populate();

		gtk_icon_view_set_model(GTK_ICON_VIEW(view),model);
		gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(view),GTK_SELECTION_SINGLE);
		gtk_icon_view_set_text_column(GTK_ICON_VIEW(view),COL_NAME);
		gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(view),COL_PIXBUF);

		return view;
	}

	int SearchWindow :: openImage(std::string p, int i )
	{
		resizeData *mod = &image;
		if(i == 2)
			mod = &image2;

		mod->image =  GTK_WIDGET(gtk_builder_get_object(builder,"image5"));
		mod->viewport = GTK_WIDGET(gtk_builder_get_object(builder,"viewport5"));
		mod->aspect =  GTK_WIDGET(gtk_builder_get_object(builder,"aspectframe5"));
		mod->sourcePixbuf = gdk_pixbuf_new_from_file(p.c_str(),NULL);
		gtk_image_set_from_pixbuf(GTK_IMAGE(image.image),gdk_pixbuf_copy(image.sourcePixbuf));

	
		int width = gdk_pixbuf_get_width(mod->sourcePixbuf);
		int height = gdk_pixbuf_get_height(mod->sourcePixbuf);

		float ratio = width / (float) height;


		gtk_aspect_frame_set(GTK_ASPECT_FRAME(mod->aspect),0,0,ratio,FALSE);


		g_signal_connect(mod->viewport,"size-allocate",G_CALLBACK(sizeChanged),mod);



		
		return 0;
	}



	int SearchWindow :: createWindow()
	{
		GtkWidget *window;


		builder = gtk_builder_new();
		gtk_builder_add_from_file(builder,"window.glade",NULL);

		window = GTK_WIDGET(gtk_builder_get_object(builder,"window3"));
		view = GTK_WIDGET(gtk_builder_get_object(builder,"iconview1"));
		entry = GTK_WIDGET(gtk_builder_get_object(builder,"entry1"));
		scroll = GTK_WIDGET(gtk_builder_get_object(builder,"scrolledwindow4"));
		//openImage("test.png");

		create_view_and_model();

		g_signal_connect(view,"item-activated",G_CALLBACK(item_activated),this);
		g_signal_connect (entry, "activate",G_CALLBACK (entry_activated),this);
		g_signal_connect(scroll,"scroll-child",G_CALLBACK(scrolled),this);

		gtk_widget_show_all(window);
		gtk_main();

		return 1;
	}

	GtkTreeModel * SearchWindow :: populate()
	{


		population = base->collectImages();
		GtkTreeIter iter;
		store = gtk_list_store_new(NUM_COLS,MetaData,G_TYPE_STRING,GDK_TYPE_PIXBUF);

		for(auto i : *population){

		GdkPixbuf *stock = gdk_pixbuf_new_from_file(i.filePath().string().c_str(),NULL);
		int width = gdk_pixbuf_get_width(stock);
		int height = gdk_pixbuf_get_height(stock);

		float ratio = width / (float) height;


		pixbuf = gdk_pixbuf_scale_simple(stock,64,64.0 /ratio , GDK_INTERP_BILINEAR);


		gtk_list_store_append(store,&iter);
		gtk_list_store_set(store,&iter,
							COL_METADATA,i,
							COL_NAME,i.fileName().string().c_str(),
							COL_PIXBUF,pixbuf,
							-1);


			}


		return GTK_TREE_MODEL (store);

	}
	GtkTreeModel * SearchWindow :: populate(std::string t)
	{
		base->filter(t);
		population = base->collectImages();

		GtkTreeIter iter;
		store = gtk_list_store_new(NUM_COLS,MetaData,G_TYPE_STRING,GDK_TYPE_PIXBUF);


		for(auto i : *population){

		GdkPixbuf *stock = gdk_pixbuf_new_from_file(i.filePath().string().c_str(),NULL);
		int width = gdk_pixbuf_get_width(stock);
		int height = gdk_pixbuf_get_height(stock);

		float ratio = width / (float) height;


		pixbuf = gdk_pixbuf_scale_simple(stock,64,64.0 /ratio , GDK_INTERP_BILINEAR);
		//pixbuf = gdk_pixbuf_new_from_file("icon.png",NULL);


		gtk_list_store_append(store,&iter);
		gtk_list_store_set(store,&iter,
							COL_METADATA,i,
							COL_NAME,i.fileName().string().c_str(),
							COL_PIXBUF,pixbuf,
							-1);


				}


		gtk_icon_view_set_model(GTK_ICON_VIEW(view),GTK_TREE_MODEL(store));
		gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(view),GTK_SELECTION_SINGLE);
		gtk_icon_view_set_text_column(GTK_ICON_VIEW(view),COL_NAME);
		gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(view),COL_PIXBUF);


		return GTK_TREE_MODEL (store);
	}





}
	
*/
