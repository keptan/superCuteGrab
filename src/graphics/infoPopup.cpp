#include "infoPopup.h" 
#include <glib.h>
#include <glib/gi18n.h>





InfoPopup :: InfoPopup ( const Glib::RefPtr<Gtk::Builder> b 
					   , cute::CollectionMan& c
					   , std::shared_ptr< cute::Image> i)

   :  image(i), collection(c), builder(b)
   , infoImage(builder, "infoAspect", "infoScroll", "infoImage")
{
	m_refTreeModel = Gtk::ListStore::create(m_Columns);
		
	builder->get_widget("infoWindow", window);
	builder->get_widget("scoreLabel", scoreLabel);
	builder->get_widget("infoTags", tagTree);

	tagTree->set_model(m_refTreeModel);
	tagTree->show();
	infoImage.setImage( image);
	setImage(image);
	
}

InfoPopup :: ~InfoPopup (void)
{
	window->hide();
}

Gtk::Window* InfoPopup :: getWindow (void)
{
	return window; 
}

void InfoPopup :: setImage (std::shared_ptr< cute::Image> i)
{
	image = i;
	infoImage.setImage(i);
	scoreLabel->set_text( "IdentityScore: "  + std::to_string( collection.getSkill(i).mu));

	m_refTreeModel->clear();
	Gtk::TreeModel::Row r = *(m_refTreeModel->append());
	r[m_Columns.m_col_name] = i->location.string();
	std::cout << i->location.string() << "set" << std::endl;
}

	
BrowseWindow :: BrowseWindow 
( const Glib::RefPtr<Gtk::Builder> b , cute::CollectionMan& c, cute::HashDB& h)
	: fight(b,c), builder(b),  collection(c), hash(h), thumbnails("thumbnails")
{
	//set up drag and drop 

	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	builder->get_widget("browseWindow", window);

	std::vector<Gtk::TargetEntry> listTargets; 
	listTargets.push_back(Gtk::TargetEntry("text/uri-list")); 
	window->drag_dest_set(listTargets);
	window->signal_drag_data_received().connect(sigc::mem_fun(*this, &BrowseWindow::on_dropped_file)); 



	Gtk::ScrolledWindow* scroll;
	builder->get_widget("browseScroll", scroll);
	scroll->add(view);
	view.show();
	
	//builder->get_widget("browseIcon", view);
	view.set_model(m_refTreeModel);
	view.set_pixbuf_column(m_Columns.m_col_pixbuf);
	view.set_markup_column(m_Columns.m_col_name);
	view.signal_item_activated().connect( sigc::mem_fun(*this, &BrowseWindow::selected));

	std::vector<Gtk::TargetEntry> listSources; 
	listSources.push_back(Gtk::TargetEntry("text/uri-list")); 
	view.drag_source_set( listSources); 
	view.signal_drag_data_get().connect(sigc::mem_fun(*this, &BrowseWindow::get_selected_data));



	Gtk::Button* button; 
	builder->get_widget("importButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, 
				&BrowseWindow::import_folder) );

	view.signal_image().connect(sigc::mem_fun(*this, &BrowseWindow::callback));



	iPop = nullptr;

	for(auto &i : collection.getImages())
		addMember(i);

}

void BrowseWindow :: addMember (const std::shared_ptr<cute::Image> i)
{
	Gtk::TreeModel::Row r = *(m_refTreeModel->append());

	r[m_Columns.m_col_name] = i->location.string();
	r[m_Columns.m_col_pixbuf] = Gdk::Pixbuf::create_from_file( thumbnails.getThumbPath(*i).c_str());
	r[m_Columns.m_col_image] = i;
}	


	

void BrowseWindow :: callback (const std::vector<Gtk::TreeModel::Path> paths)
{
	if(paths.size())
	{
	const auto path = paths.back();

	std::cout << "received signal" << std::endl;
	auto iter = m_refTreeModel->get_iter(path);
	auto row = *iter; 

	std::shared_ptr<cute::Image>  image = row[m_Columns.m_col_image];
	std::cout << image->location << '\n';


	if(iPop == nullptr)
		iPop = std::make_unique<InfoPopup>( builder, collection, image);

	iPop->setImage(image);
	iPop->getWindow()->show_all_children();
	iPop->getWindow()->show();


	}
}

Gtk::Window* BrowseWindow :: getWindow (void)
{
	return window; 
}

void BrowseWindow :: selected (const Gtk::TreeModel::Path& path)
{
	auto iter = m_refTreeModel->get_iter(path);
	auto row = *iter; 

	std::shared_ptr<cute::Image>  image = row[m_Columns.m_col_image];
	std::cout << image->location << '\n';

	collection.setLeftImage(image);
	fight.refresh();
	fight.getWindow()->show_all_children();
	fight.getWindow()->show();

	m_refTreeModel->clear();
	for(auto &i : collection.getImages())
		addMember(i);


}

void BrowseWindow :: import_folder (void)
{
	  Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
	  dialog.set_transient_for(*window);

	  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	  dialog.add_button("Select", Gtk::RESPONSE_OK);

	  int result = dialog.run();
	switch(result)
	  {
		case(Gtk::RESPONSE_OK):
		{
		  std::cout << "Select clicked." << std::endl;
		  std::cout << "Folder selected: " << dialog.get_filename()
			  << std::endl;
		  hash.scanDirectory(dialog.get_filename());

		  auto i = collection.getImages();



			for(auto &f : std::filesystem::directory_iterator(dialog.get_filename()))
			{
				if(!cute::conformingFileType(f.path())) continue;
				i.push_back( std::make_shared<cute::Image> (f.path(), hash.retrieveData(f.path())));
			}

			collection.setImages(i);
			m_refTreeModel->clear();
			for(auto &i : collection.getImages())
				addMember(i);





		  break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
		  std::cout << "Cancel clicked." << std::endl;
		  break;
		}
		default:
		{
		  std::cout << "Unexpected button clicked." << std::endl;
		  break;
		}
	  }

		return;
}


void BrowseWindow :: on_dropped_file(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time)
{
	if ((selection_data.get_length() >= 0) && (selection_data.get_format() == 8))
	 {
			 std::vector<Glib::ustring> file_list;
	 
			 file_list = selection_data.get_uris();
	 
		 if (file_list.size() > 0)
		 {
			 for(auto &file : file_list)
			 {
				 Glib::ustring path = Glib::filename_from_uri(file);
				 //do something here with the 'filename'. eg open the file for reading
				 context->drag_finish(true, false, time);
			 }
			 return;
		 }
	 }
 
	 context->drag_finish(false, false, time);
}

void BrowseWindow :: get_selected_data (
		   const Glib::RefPtr<Gdk::DragContext>& context, 
			Gtk::SelectionData& selection_data, guint info, guint time)
{
	std::vector<Glib::ustring> paths; 


	const auto selection = view.get_selected_items(); 
	if(selection.size() > 0)
	{
		auto iter = m_refTreeModel->get_iter(selection[0]);
		auto row = *iter; 

		std::shared_ptr<cute::Image> image = row[m_Columns.m_col_image];

		paths.push_back(  g_filename_to_uri( std::filesystem::absolute(image->location).string().c_str(), NULL, NULL));
		selection_data.set_pixbuf( row[m_Columns.m_col_pixbuf]);
		selection_data.set_uris(paths);
	}

}

