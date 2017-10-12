#include <gtkmm.h>
#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include "scalingImage.h"
#include "../metaData.h"
#include "../imageBase.h"
#include "searchWindow.h"



namespace cute
{
	SearchWindow :: SearchWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder> &refGlade)
		:Gtk::Window(cobject)
		,builder(refGlade)
	{

		image = new resizeData;
		image2 = new resizeData;

		builder->get_widget("iconview1",iconView);
		builder->get_widget("entry1",entry);
		builder->get_widget("iconScroll",iconScroll);

		builder->get_widget("view",image->view);
		builder->get_widget("scrollView",image->scrollView);
		builder->get_widget("image",image->image);
		builder->get_widget("aspect",image->aspect);

		builder->get_widget("view1",image2->view);
		builder->get_widget("scrollView1",image2->scrollView);
		builder->get_widget("image1",image2->image);
		builder->get_widget("aspect1",image2->aspect);



		//image2->view->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::rightsizeChanged));
		image->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::sizeChanged));
		//image2->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::rightsizeChanged));


		refListModel = Gtk::ListStore::create(m_Columns);
	}

	void SearchWindow :: baseInit(ImageBase *b)
	{

		base = b;
		base->filter("");
		population = base->collectImages();


		iconView->set_model(refListModel);
		iconView->set_markup_column(m_Columns.m_col_name);
		iconView->set_pixbuf_column(m_Columns.m_col_pixbuf);
		iconView->signal_item_activated().connect(sigc::mem_fun(*this,&SearchWindow::on_item_activated));
		iconView->signal_scroll_event().connect(sigc::mem_fun(*this,&SearchWindow::on_scroll));

		//iconScroll->signal_scroll_child().connect(sigc::mem_fun(*this,&SearchWindow::on_scroll));
		entry->signal_activate().connect(sigc::mem_fun(*this,&SearchWindow::entry_activated));



	}

	void SearchWindow :: add_entry(const std::string &filename, int loc)
	{


		MetaData data = (*population)[loc];
		Glib::RefPtr<Gdk::Pixbuf> buf = Gdk::Pixbuf::create_from_file("icon.png");

		auto row =*(refListModel->append());
		row[m_Columns.m_col_name] = filename.substr(0,0);
		row[m_Columns.m_col_path] = data.filePath().string();
		row[m_Columns.m_col_data] = loc;
		row[m_Columns.m_col_pixbuf] = buf;

		row[m_Columns.m_col_thread] = 0;

		row[m_Columns.m_col_hasThumb] = 0;
	}


	void SearchWindow :: on_item_activated(const Gtk::TreeModel::Path &path)
	{

		auto iter = refListModel->get_iter(path);
		auto row = *iter;

		int i = row[m_Columns.m_col_data];
		const std::string loc = row[m_Columns.m_col_path];
		const std::string name = row[m_Columns.m_col_path];

		busy = true;


		image->data = new MetaData((*population)[i]);
	   	image2->data = new MetaData(base->findMatch(*image->data));


		newImageBox(image->data->filePath().string());
		rightImageBox(image2->data->filePath().string());


		this->signal_key_press_event().connect(sigc::mem_fun(*this, &SearchWindow::onKeyPress), false);

		image->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::sizeChangedII));
		image2->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::rightsizeChangedII));
		busy = false;

		/*
		delete(apsi);
		apsi = new 	AspectPreservingScalingImage(Gdk::Pixbuf::create_from_file(localData.filePath().string()));
		imageWindow->add(*apsi);
		imageWindow->show_all();
		*/



	}

	bool SearchWindow :: addThumb(Gtk::TreeModel::Path path)
	{

			m.lock();
			Glib::RefPtr<Gdk::Pixbuf> buf;	
			double ratio;

			auto iter = refListModel->get_iter(path);
			auto row = *iter;

			int i = row[m_Columns.m_col_data];
			const std::string loc = row[m_Columns.m_col_path];
			const std::string name = row[m_Columns.m_col_path];
			row[m_Columns.m_col_thread] = 1;

			if(!row[m_Columns.m_col_hasThumb]){

			buf  = Gdk::Pixbuf::create_from_file(loc);
			m.unlock();
			ratio = (double) buf->get_width() / buf->get_height();
			buf = buf->scale_simple(64,64.0/ratio,Gdk::INTERP_BILINEAR);
			m.lock();

			row[m_Columns.m_col_pixbuf] = buf;
			row[m_Columns.m_col_hasThumb] = 1;
			}
			m.unlock();

			return true;

	}



	bool SearchWindow :: on_scroll(GdkEventScroll *e)
	{
		Gtk::TreeModel::Path start;
		Gtk::TreeModel::Path end;

		Glib::RefPtr<Gdk::Pixbuf> buf;	
		iconView->get_visible_range(start,end);
		double ratio;
		for(int i = 0;i<=5;i++)
			end.next();
	
		
		for(;start <= end;start.next())
		{

			/*
			auto iter = refListModel->get_iter(start);
			auto row = *iter;
			for(auto &i:threads)
				i->join();

			if(!row[m_Columns.m_col_hasThumb]){
			row[m_Columns.m_col_thread] = 1;
			threads.push_back(new std::thread(sigc::mem_fun(*this,&SearchWindow::addThumb),start));
			}
			*/
			auto iter = refListModel->get_iter(start);
			auto row = *iter;

			int i = row[m_Columns.m_col_data];
			const std::string loc = row[m_Columns.m_col_path];
			const std::string name = row[m_Columns.m_col_path];

			if(!row[m_Columns.m_col_hasThumb]){

			buf  = Gdk::Pixbuf::create_from_file(loc);
			ratio = (double) buf->get_width() / buf->get_height();
			buf = buf->scale_simple(84,84.0/ratio,Gdk::INTERP_BILINEAR);

			row[m_Columns.m_col_pixbuf] = buf;
			row[m_Columns.m_col_hasThumb] = 1;

			}


		}
		return false;
	}





		
	void SearchWindow :: entry_activated()
	{
		std::string data = entry->get_text();
		populate(data);

		for(auto &i:threads)
			i->join();


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
			add_entry(localData.fileName(),i);
			++i;
		}


		//add_entry("test.png",1);
	}


	void SearchWindow :: sizeChanged(Gtk::Allocation& a)
	{
		if(busy)
			return;

		Gtk::Allocation& allocation = a;

		Gtk::Image * imagePixbuf = image->image;

		
		if(sized){
		
		if (allocation.get_width() -4  != image->image->get_width() -4 ||
			allocation.get_height() -4 !=  image->image->get_width()-4){

				image->image->set(image->sourcePixbuf->scale_simple(
				allocation.get_width() -4,
				allocation.get_height() -4,
				Gdk::INTERP_BILINEAR));

				 imagePixbuf = image2->image;
				 allocation = image2->scrollView->get_allocation();

		
				image2->image->set(image2->sourcePixbuf->scale_simple(
				allocation.get_width() -4,
				allocation.get_height() -4,
				Gdk::INTERP_BILINEAR));

		}
		sized = false;
		return;
		}
		sized = true;
		return;
	}
	void SearchWindow :: mansizeChanged()
	{
		if(busy)
			return;

		Gtk::Allocation allocation;
		allocation= image->scrollView->get_allocation();

		Gtk::Image * imagePixbuf = image->image;

		
		if(sized){
		
		if (allocation.get_width() -4  != image->image->get_width() -4 ||
			allocation.get_height() -4 !=  image->image->get_width()-4){

				image->image->set(image->sourcePixbuf->scale_simple(
				allocation.get_width() -4,
				allocation.get_height() -4,
				Gdk::INTERP_BILINEAR));

				 imagePixbuf = image2->image;
				 allocation = image2->scrollView->get_allocation();

		
				image2->image->set(image2->sourcePixbuf->scale_simple(
				allocation.get_width() -4,
				allocation.get_height() -4,
				Gdk::INTERP_BILINEAR));

		}
		sized = false;
		return;
		}
		sized = true;
		return;
	}



/*	void SearchWindow :: rightsizeChanged(Gtk::Allocation& allocation)
	{
		if(busy)
			return;

	
		}
		sized = false;
		return;
		}
		sized = true;
		return;
	}
	*/
	void SearchWindow :: sizeChangedII(Gtk::Allocation& allocation)
	{
	
		return;
	}
	void SearchWindow :: rightsizeChangedII(Gtk::Allocation& allocation)
	{
	
		return;
	}
	void SearchWindow :: imageClicked()
	{
	
		return;
	}


	resizeData *SearchWindow :: newImageBox(std::string i)
	{
				
		busy = true;
		image->sourcePixbuf = Gdk::Pixbuf::create_from_file(i);
	

		int width = image->sourcePixbuf->get_width();
		int height = image->sourcePixbuf->get_height();
		float ratio = (float)width/ (float) height;


		image->aspect->set(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,ratio,FALSE);
		image->aspect->set_shadow_type(Gtk::SHADOW_NONE);
		image->image->set(image->sourcePixbuf);

		
		return image;

	}

	resizeData *SearchWindow :: rightImageBox(std::string i)
	{
				
		busy = true;
		image2->sourcePixbuf = Gdk::Pixbuf::create_from_file(i);
	

		int width = image2->sourcePixbuf->get_width();
		int height = image2->sourcePixbuf->get_height();
		float ratio = (float)width/ (float) height;


		image2->aspect->set(Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER,ratio,FALSE);
		image2->aspect->set_shadow_type(Gtk::SHADOW_NONE);
		image2->image->set(image2->sourcePixbuf);

		
		return image2;

	}

	bool SearchWindow :: onKeyPress(GdkEventKey *event)
	{
		if( event->hardware_keycode == 113){
			   std::cout<<"left key";
		mansizeChanged();

			leftCount++;
			rightCount = 0;

			base->runElo(*image->data,*image2->data);

			image2->data = new MetaData(base->findMatch(*image->data,leftCount));
			rightImageBox(image2->data->filePath().string());

			if(leftCount > 4){

			leftCount = 0;
			image->data = new MetaData(base->findMatch(*image2->data,leftCount));
	newImageBox(image->data->filePath().string());
			}





	

		image->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::sizeChangedII));
		image2->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::rightsizeChangedII));
		busy = false;
		mansizeChanged();

					return true;
			   
			}

	if( event->hardware_keycode == 111){
			   std::cout<<"up? ";
			mansizeChanged();

			rightCount = 0;
			leftCount = 0;

			base->runElo(*image->data,*image2->data,-1);

			image2->data = new MetaData(base->findMatch(*image->data,-1));
			rightImageBox(image2->data->filePath().string());
			image->data = new MetaData(base->findMatch(*image2->data,-1));
			newImageBox(image->data->filePath().string());


			//base->runElo(*image->data,*image2->data);

	

		image->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::sizeChangedII));
		image2->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::rightsizeChangedII));
		busy = false;
		mansizeChanged();

					return true;
			   
			}

	if( event->hardware_keycode == 27){
			   std::cout<<"up? ";
			mansizeChanged();

			rightCount = 0;
			leftCount = 0;

			//base->runElo(*image->data,*image2->data,-1);

			image2->data = new MetaData(base->findMatch(*image->data,-1));
			rightImageBox(image2->data->filePath().string());
			image->data = new MetaData(base->findMatch(*image2->data,-1));
			newImageBox(image->data->filePath().string());


			//base->runElo(*image->data,*image2->data);

	

		image->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::sizeChangedII));
		image2->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::rightsizeChangedII));
		busy = false;
		mansizeChanged();

					return true;
			   
			}

		if( event->hardware_keycode == 114){
			   std::cout<<"right key";


			rightCount++;
			leftCount = 0;
			base->runElo(*image2->data,*image->data);


			image->data = new MetaData(base->findMatch(*image2->data,rightCount));
			newImageBox(image->data->filePath().string());

			if(rightCount > 4){
			rightCount = 0;
			image2->data = new MetaData(base->findMatch(*image->data,rightCount));
			rightImageBox(image2->data->filePath().string());
			}




		image->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::sizeChangedII));
		image2->scrollView->signal_size_allocate().connect(sigc::mem_fun(*this,&SearchWindow::rightsizeChangedII));
		busy = false;

		mansizeChanged();

					return true;

			   
			}
		if( event->hardware_keycode == 65 || event->hardware_keycode == 116){
			 		mansizeChanged();
					return true;

			   
			}
		else{
			std::cout<<event->hardware_keycode<<'\n';
		}
		
			
	return false;

	}


};
/*

gboolean  resize_image(GtkWidget *widget, GdkEvent *event, GtkWindow *window)
	{
		GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(widget));
		
		pixbuf = gdk_pixbuf_scale_simple(pixbuf,widget->allocation.width -2,widget->allocation.height -2, GDK_INTERP_BILINEAR);

		gtk_image_set_from_pixbuf(GTK_IMAGE(widget),pixbuf);

		return FALSE;
	}

	gboolean sizeChanged(GtkWidget *widget, GtkAllocation *allocation, resizeData *data)
	{
		GdkPixbuf *sourcePixbuf = data->sourcePixbuf;
		GdkPixbuf *imagePixbuf;

		imagePixbuf = gtk_image_get_pixbuf(GTK_IMAGE(data->image));
		
		if (allocation->width -2  != gdk_pixbuf_get_width(imagePixbuf)||
				allocation->height -2 != gdk_pixbuf_get_height(imagePixbuf)){
			gtk_image_set_from_pixbuf(
					GTK_IMAGE(data->image),
					gdk_pixbuf_scale_simple(sourcePixbuf,
						allocation->width -2,
						allocation->height -2,
						GDK_INTERP_BILINEAR)
			);
				g_object_unref(imagePixbuf);
		}
		return FALSE;
	}


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
