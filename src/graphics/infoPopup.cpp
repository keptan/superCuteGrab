#include "infoPopup.h" 
#include <algorithm>
#include <glib.h>
#include <glib/gi18n.h>



namespace graphics
{

InfoPopup :: InfoPopup ( const Glib::RefPtr<Gtk::Builder> b 
					   , cute::ThumbDB& t, cute::CollectionMan& c)

   :   thumbnails(t), collection(c), builder(b)
   , infoImage(builder, "infoAspect", "infoScroll", "infoImage")
{
	//setup builder widgets
	builder->get_widget("infoWindow", window);
	builder->get_widget("infoTags", tagTree);
	builder->get_widget("addTag", addTag);
	builder->get_widget("infoAspect", frame);
	builder->get_widget("infoIconScroll", scroll);

	builder->get_widget("character", character);
	builder->get_widget("artist", artist);
	builder->get_widget("viewCharacter", viewCharacter);
	builder->get_widget("viewArtist", viewArtist);

	//setup iconView and tagTree view models
	//tagTree is a list of all the tags and scores of selected images
	//iconTreeModel is an optionally displayed icon view, for when we select more than one image
	tagTreeModel = Gtk::ListStore::create(tagColumns);
	iconTreeModel = Gtk::ListStore::create( iconColumns);

		
	//setup the columns for the tagTree list and iconView
	tagTree->set_model(tagTreeModel);
	tagTree->append_column("score", tagColumns.m_col_score);
	tagTree->append_column("ID", tagColumns.m_col_name);

	icons.set_model(iconTreeModel);
	icons.set_pixbuf_column(iconColumns.m_col_pixbuf);

	scroll->add(icons);

	//setting up autocomplete for the user tag entry
	auto completion1 = Gtk::EntryCompletion::create();
	auto completion2 = Gtk::EntryCompletion::create();
	auto completion3 = Gtk::EntryCompletion::create();

	addTag->set_completion(completion1);
	addTag->signal_activate().connect(sigc::mem_fun(*this,
      &InfoPopup::insertTag) );

	artist->set_completion(completion2);
	artist->signal_activate().connect(sigc::mem_fun(*this, 
	  &InfoPopup::insertArtist) );

	character->set_completion(completion3);
	character->signal_activate().connect(sigc::mem_fun(*this, 
	  &InfoPopup::insertCharacter) );

	auto refCompletionModel1 = Gtk::ListStore::create(c_Columns);
	auto refCompletionModel2 = Gtk::ListStore::create(c_Columns);
	auto refCompletionModel3 = Gtk::ListStore::create(c_Columns);

	completion1->set_model(refCompletionModel1);
	completion2->set_model(refCompletionModel2);
	completion3->set_model(refCompletionModel3);

	//appending a test item, placeholder
	int i = 0;
	for(const auto t : collection.booruTags.tags.retrieveData())
	{
		Gtk::TreeModel::Row row = *(refCompletionModel1->append());
		row[c_Columns.m_col_id] = i++;
		row[c_Columns.m_col_name] = t.tag;
	}

	i = 0;
	for(const auto t : collection.artistTags.tags.retrieveData())
	{
		Gtk::TreeModel::Row row = *(refCompletionModel2->append());
		row[c_Columns.m_col_id] = i++;
		row[c_Columns.m_col_name] = t.tag;
	}

	i = 0;
	for(const auto t : collection.charTags.tags.retrieveData())
	{
		Gtk::TreeModel::Row row = *(refCompletionModel3->append());
		row[c_Columns.m_col_id] = i++;
		row[c_Columns.m_col_name] = t.tag;
	}

	completion1->set_text_column(c_Columns.m_col_name);
	completion2->set_text_column(c_Columns.m_col_name);
	completion3->set_text_column(c_Columns.m_col_name);

	tagTree->show();

	//insert the icons widget into the scrollbar
}

InfoPopup :: ~InfoPopup (void)
{
	window->hide();
}

Gtk::Window* InfoPopup :: getWindow (void)
{
	return window; 
}

void InfoPopup :: insertTag (void)
{
	const cute::Tag t = addTag->get_text().raw();
	std::cout << t.tag << std::endl;
	addTag->set_text("");

	Gtk::TreeModel::Row r = *(tagTreeModel->append());
	r[tagColumns.m_col_name]		= t.tag;
	r[tagColumns.m_col_score]		= collection.booruTags.scores.retrieveData(t).skill();

	for(const auto i : selected)
	{
		collection.booruTags.insert(*i, t);
	}
}

void InfoPopup :: insertArtist (void)
{
	const cute::Tag t = artist->get_text().raw();
	std::cout << t.tag << std::endl;
	artist->set_text("");

	viewArtist->set_text( viewArtist->get_text().raw() + ", " + t.tag);

	for(const auto i : selected)
	{
		collection.artistTags.insert(*i, t);
	}
}

void InfoPopup :: insertCharacter (void)
{
	const cute::Tag t = character->get_text().raw();
	std::cout << t.tag << std::endl;
	character->set_text("");

	viewCharacter->set_text( viewCharacter->get_text().raw() + ", " + t.tag);

	for(const auto i : selected)
	{
		collection.charTags.insert(*i, t);
	}
}

/*
void InfoPopup :: setImage (std::shared_ptr< cute::Image> i)
{
	image = i;
	infoImage.setImage(i);

	m_refTreeModel->clear();

	Gtk::TreeModel::Row r = *(m_refTreeModel->append());
	r[m_Columns.m_col_name]		= i->location.filename().string();
	r[m_Columns.m_col_score]	= collection.getSkill(i).skill();

	infoImage.show();
	scroll->hide();
}
*/

void InfoPopup :: setImages (const std::vector< std::shared_ptr< cute::Image>> i)
{
	selected = i;



	//clear the iconview of all icons
	iconTreeModel->clear();
	if(!selected.size()) return;

	infoImage.setImage( i[0]);

	const auto sortedTagCollection = [&](auto& db)
	{
		cute::TagSet acc = db.getTags(*i[0]); 
		for( auto im : i) acc = acc & db.getTags(*im);

		std::vector<cute::Tag> sorted;
		sorted.insert(sorted.begin(), acc.begin(), acc.end());
		std::sort(sorted.begin(), sorted.end(),
			  [&](const auto a, const auto b)
			  {
				 return db.scores.retrieveData(a).skill() > 
				 db.scores.retrieveData(b).skill();
			});

		return sorted;
	};


	tagTreeModel->clear();

	if(selected.size() == 1)
	{
		Gtk::TreeModel::Row r     = *(tagTreeModel->append());
		r[tagColumns.m_col_name]  = i[0]->location.filename().string();
		r[tagColumns.m_col_score] = collection.identityRanker.getSkill(*i[0]).skill();
	}

	for(const auto t : sortedTagCollection( collection.booruTags))
	{
		Gtk::TreeModel::Row r = *(tagTreeModel->append());
		r[tagColumns.m_col_name]		= t.tag;
		r[tagColumns.m_col_score]	= collection.booruTags.scores.retrieveData(t).skill();
	}

	//time to get the char tags and stuff
	std::string charString;
	for(const auto t : sortedTagCollection( collection.charTags))
	{
		std::cout << t.tag << std::endl;
		charString += t.tag + ',';
	}
	viewCharacter->set_text(charString.c_str());

	//time to get the char tags and stuff
	std::string artistString;
	for(const auto t : sortedTagCollection( collection.artistTags))
	{
		std::cout << t.tag << std::endl;
		artistString += t.tag + ',';
	}
	viewArtist->set_text(artistString.c_str());





	//if only one image, show the large preview instead of the icon view
	if(selected.size() == 1)
	{
		//set the image to the first image
		infoImage.show();
		scroll->hide();
		icons.hide();
		return;
	}

	for(const auto im : i)
	{
		//append all the images into the iconView widget
		Gtk::TreeModel::Row r = *(iconTreeModel->append());

		r[iconColumns.m_col_name] = im->location.string();
		r[iconColumns.m_col_pixbuf] = Gdk::Pixbuf::create_from_file( thumbnails.getThumbPath(*im).c_str());
		r[iconColumns.m_col_image] = im; 
	}




	scroll->show();
	infoImage.hide();
	icons.show();
}

	
BrowseWindow :: BrowseWindow 
( const Glib::RefPtr<Gtk::Builder> b , cute::CollectionMan& c, cute::HashDB& h)
	: fight(b,c), builder(b),  collection(c), hash(h), thumbnails("thumbnails"), iPop(nullptr)
{
	//setup a pointer to our own window
	builder->get_widget("browseWindow", window);

	//setup the tagTree and tagEntry which we dont use until later
	builder->get_widget("tagTree", tagTree);

	tagTreeModel = Gtk::ListStore::create(tagColumns);
	tagTree->set_model(tagTreeModel);
	tagTree->append_column("score", tagColumns.m_col_score);
	tagTree->append_column("ID", tagColumns.m_col_name);

	auto sel = tagTree->get_selection();
	sel->set_mode(Gtk::SELECTION_MULTIPLE);

	//tagTree->signal_row_activated().connect( sigc::mem_fun(*this, &BrowseWindow::on_dropped_file));

	//we use the window pointer to help setup drag and drop
	std::vector<Gtk::TargetEntry> listTargets; 
	listTargets.push_back(Gtk::TargetEntry("text/uri-list")); 
	window->drag_dest_set(listTargets);
	window->signal_drag_data_received().connect(sigc::mem_fun(*this, &BrowseWindow::on_dropped_file)); 

	//setting up the refTreeModel for the icons 
	m_refTreeModel = Gtk::ListStore::create(m_Columns);

	//add our icon view to the scrollbox
	Gtk::ScrolledWindow* scroll;
	builder->get_widget("browseScroll", scroll);
	scroll->add(view);

	//setup the icon view columns, and show it
	view.set_model(m_refTreeModel);
	view.set_pixbuf_column(m_Columns.m_col_pixbuf);
	view.set_markup_column(-1);

	//callbacks and drag and drop out
	view.signal_item_activated().connect( sigc::mem_fun(*this, &BrowseWindow::selected));

	//further setup for drag and drop
	std::vector<Gtk::TargetEntry> listSources; 
	listSources.push_back(Gtk::TargetEntry("text/uri-list")); 
	view.drag_source_set( listSources); 
	view.signal_drag_data_get().connect(sigc::mem_fun(*this, &BrowseWindow::get_selected_data));

	//show the icon view here
	view.show();

	//setup some callbacks for different buttons
	Gtk::Button* button; 
	builder->get_widget("importButton", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, 
				&BrowseWindow::import_folder) );

	view.signal_image().connect(sigc::mem_fun(*this, &BrowseWindow::callback));

	builder->get_widget("importButton2", button);
	button->signal_clicked().connect(sigc::mem_fun(*this, 
				&BrowseWindow::import_folder_recursive) );

	builder->get_widget("filterTag", button); 
	button->signal_clicked().connect(sigc::mem_fun(*this, 
				&BrowseWindow::filterTagTree) ); 

	builder->get_widget("clearTag", button); 
	button->signal_clicked().connect(sigc::mem_fun(*this, 
				&BrowseWindow::refresh) ); 

	Gtk::ComboBoxText* combo;
	builder->get_widget("sortBox", combo);
	combo->signal_changed().connect(sigc::mem_fun(*this, 
				&BrowseWindow::refresh) );

	builder->get_widget("filterSearch", filterSearch);
	filterSearch->signal_activate().connect(sigc::mem_fun(*this, &BrowseWindow::filter) );

	for(auto &i : collection.getImages())
		addMember(i);

}

//add a image to the icon view
void BrowseWindow :: addMember (const std::shared_ptr<cute::Image> i)
{

	Gtk::TreeModel::Row r = *(m_refTreeModel->append());

	const std::string loc = i->location.filename().string();
	const std::string cut = loc.size() > 25 ? loc.substr(0,24) : loc;

	//basic treeModel column stuff we do all the time
	r[m_Columns.m_col_name] = cut;
	r[m_Columns.m_col_pixbuf] = Gdk::Pixbuf::create_from_file( thumbnails.getThumbPath(*i).c_str());
	r[m_Columns.m_col_image] = i;
}	

void BrowseWindow :: comboSort (std::vector<cute::SharedImage>& images)
{
	std::map<std::string, std::function< bool (cute::SharedImage, cute::SharedImage)>> lamMap;

	lamMap["Score"]        = [&](const auto a, const auto b)
						   {
								return collection.identityRanker.getSkill(*a).skill() > 
									   collection.identityRanker.getSkill(*b).skill();
						   };

	lamMap["#Characters"]  = [&](const auto a, const auto b)
						   {
								return collection.charTags.getTags(*a).size() < 
									   collection.charTags.getTags(*b).size();
						   };

	lamMap["Date"]		    = [&](const auto a, const auto b)
						   {
								return std::filesystem::last_write_time(a->location) > 
									   std::filesystem::last_write_time(b->location);
						   };


	Gtk::ComboBoxText* combo;
	builder->get_widget("sortBox", combo);
	const std::string boxStr = combo->get_active_text().raw();

	std::sort(images.begin(), images.end(), lamMap[boxStr]);
}

//callback for the right click 'info' menu
void BrowseWindow :: callback (const std::vector<Gtk::TreeModel::Path> paths)
{
	//vector we fill with images
	//wish I knew how to pass around ref columns themselves or something 
	std::vector< std::shared_ptr< cute::Image>> images;

	for(const auto p : paths)
	{
		auto iter = m_refTreeModel->get_iter(p);
		auto row = *iter; 

		std::shared_ptr<cute::Image>  image = row[m_Columns.m_col_image];
		images.push_back(image);
	}

	//if a window isn't setup yet, we make one
	if(iPop == nullptr)
		iPop = std::make_unique<InfoPopup>( builder, thumbnails, collection);

	iPop->setImages(images);
	iPop->getWindow()->show();
}

//get a pointer to our window
Gtk::Window* BrowseWindow :: getWindow (void)
{
	return window; 
}

//callback for when we select an image, opens the fight window
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

//void BrowseWindow :: tagSelect (const Gtk::TreeModel::Path& path)

//importing every image in a directory
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

		refresh();

		return;
}

void BrowseWindow :: import_folder_recursive (void)
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
		  hash.scanDirectoryRecursive(dialog.get_filename());

		  auto i = collection.getImages();



			for(auto &f : std::filesystem::recursive_directory_iterator(dialog.get_filename()))
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

		refresh();

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


	
/*
void BrowseWindow :: terminate_right (void) 
{
	auto i = collection.getImages(); 
	const auto right = collection.getRightImage();
	
	i.erase( std::remove(i.begin(), i.end(), right), i.end()); 
	std::filesystem::rename( right->location, std::filesystem::path("./exile")/right->location.filename() );
	collection.setImages(i); 

	for(auto &row : m_refTreeModel->children())
	{
		auto rowImage = row.get_value(m_Columns.m_col_image);

		if(rowImage == right) m_refTreeModel->erase(row);
	}

	collection.freshImages(); 
	fight.refresh();
}
*/

void BrowseWindow :: filter (void)
{
	const std::string str = filterSearch->get_text().raw();

	collection.filter(str);

	m_refTreeModel->clear();
	for(auto &i : collection.getImages()) addMember(i);
	refreshTagTree();
}

void BrowseWindow :: refreshTagTree (void)
{

	const auto images = collection.getImages();
	if(!images.size()) return;

	auto tags	= collection.tagsWithScores(images);
	std::sort(tags.begin(), tags.end(),
			[](const auto a, const auto b)
			{ return std::get<1>(a).skill() > std::get<1>(b).skill();});

	tagTreeModel->clear();

	for(const auto t : tags)
	{
		Gtk::TreeModel::Row r = *(tagTreeModel->append());
		r[tagColumns.m_col_name]		= std::get<0>(t).tag;
		r[tagColumns.m_col_score]	= std::get<1>(t).skill();
	}

}


void BrowseWindow :: filterTagTree (void)
{
	auto sel = tagTree->get_selection();
	auto paths = sel->get_selected_rows();

	std::string search;

	for(const auto p : paths)
	{
		auto iter = tagTreeModel->get_iter(p);
		auto row = *iter;

		search += ",";
		const Glib::ustring ustr = row[tagColumns.m_col_name];
		search += ustr.raw();

	}

	collection.filter(search);
	m_refTreeModel->clear();
	for(auto &i : collection.getImages()) addMember(i);
	refreshTagTree();

}

void BrowseWindow :: refresh (void)
{
	collection.filter();

	m_refTreeModel->clear();
	auto images = collection.getImages();
	comboSort(images);
	for(auto &i : images) addMember(i);
	refreshTagTree();
}


}
