#include "browseWindow.h"
#include <chrono>

namespace graphics 
{

BrowseWindow :: BrowseWindow 
( const Glib::RefPtr<Gtk::Builder> b , cute::CollectionMan& c, cute::HashDB& h)
	: fight(b,c), builder(b),  collection(c), hash(h), thumbnails("thumbnails"),
	  defaultIcon( Gdk::Pixbuf::create_from_file( "icon.png")), cpuPool(1),syncro(1), iconContext_(1),  iPop(nullptr)
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
	m_refTreeModel   = Gtk::ListStore::create(m_Columns);

	//add our icon view to the scrollbox
	Gtk::ScrolledWindow* scroll;
	builder->get_widget("browseScroll", scroll);
	scroll->add(view);

	//setup the icon view columns, and show it
	view.set_model(m_refTreeModel);
	view.set_pixbuf_column(m_Columns.m_col_pixbuf);
	view.set_markup_column(-1);

	//callbacks and drag and drop out
	view.signal_item_activated().connect( sigc::mem_fun(*this, &BrowseWindow::openFightWindow));

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

	view.signal_image().connect(sigc::mem_fun(*this, &BrowseWindow::rightClick));
	view.signal_exile().connect(sigc::mem_fun(*this, &BrowseWindow::exileClick));

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
	filterSearch->signal_activate().connect(sigc::mem_fun(*this, &BrowseWindow::filterByEntry) );

	importCollection();
}

BrowseWindow :: ~BrowseWindow (void)
{
	cpuPool.join_abort();
}


void BrowseWindow :: importCollection (void)
{
	int d = 0;
	m_refTreeModel->clear();
	for(const auto &i : collection.getImages())
	{
		if(d++ > 100) break;
		addMember(i);
	}
}
void BrowseWindow :: importCollection ( const std::vector<cute::SharedImage>& c)
{

	int d = 0;
	m_refTreeModel->clear();

	/*
	for(const auto &i : c)
	{
		if(d++ > 100) break;
		addMember(i);
	}
	*/

	for(int i = 0; i < c.size();)
	{
		std::vector<cute::SharedImage> batch;
		for(int a = i; a < i + 1000; a++)
		{
			if(a == c.size()) break;
			batch.push_back(c[a]);
		}

		const auto batchIcons  = [&, batch, i](void)
		{
			Glib::RefPtr<Gtk::ListStore> store = m_refTreeModel;

			if(i)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(4s);
			}
			
			for(const auto image : batch)
			{
				Gtk::TreeModel::Row r = *(store->append());

				const std::string loc = image->location.filename().string();
				const std::string cut = loc.size() > 25 ? loc.substr(0,24) : loc;

				const auto buf = Gdk::Pixbuf::create_from_file( thumbnails.getThumbPath(*image).c_str());

				//basic treeModel column stuff we do all the time
				r[m_Columns.m_col_name] = cut;
				r[m_Columns.m_col_hasPixbuf] = true;
				r[m_Columns.m_col_pixbuf] = buf;
				r[m_Columns.m_col_image] = image;
			}

			m_refTreeModel = store; 
		};

		syncro.addTask(batchIcons);
		i += 1000;
	}
}

/*
void BrowseWindow :: addThumb(int context)
{
		for(auto row : m_refTreeModel->children())
		{

			{
			std::scoped_lock lk (m);
			if(context != iconContext_) return;
			}

			const bool hasBuf = row[m_Columns.m_col_hasPixbuf];
			if(hasBuf) return;


			const cute::SharedImage image = row[m_Columns.m_col_image];

			const auto buf = Gdk::Pixbuf::create_from_file( thumbnails.getThumbPath(*image).c_str());

			row[m_Columns.m_col_pixbuf] = buf;
			row[m_Columns.m_col_hasPixbuf] = true;

			return;
		}

}
*/


//add a image to the icon view
void BrowseWindow :: addMember (const std::shared_ptr<cute::Image> i)
{
	Gtk::TreeModel::Row r = *(m_refTreeModel->append());

	const std::string loc = i->location.filename().string();
	const std::string cut = loc.size() > 25 ? loc.substr(0,24) : loc;

	const auto buf = Gdk::Pixbuf::create_from_file( thumbnails.getThumbPath(*i).c_str());

	//basic treeModel column stuff we do all the time
	r[m_Columns.m_col_name] = cut;
	r[m_Columns.m_col_hasPixbuf] = true;
	r[m_Columns.m_col_pixbuf] = buf;
	r[m_Columns.m_col_image] = i;
}	

void BrowseWindow :: comboSort (std::vector<cute::SharedImage>& images)
{
	std::map<std::string, std::function< bool (cute::SharedImage, cute::SharedImage)>> lamMap;

	//memoize these!!!

	lamMap["Score"]        = [&](const auto a, const auto b)
						   {
								return collection.identityRanker.getSkill(*a).skill() > 
									   collection.identityRanker.getSkill(*b).skill();
						   };

	lamMap["#Characters"]  = [&](const auto a, const auto b)
						   {
								const auto aSize = collection.charTags.getTags(*a).size();
								const auto bSize = collection.charTags.getTags(*b).size();

								if (aSize !=  bSize) return aSize < bSize;

								const auto aTime = std::filesystem::last_write_time(a->location); 
								const auto bTime = std::filesystem::last_write_time(b->location);

								return aTime > bTime;
						   };

	lamMap["Date"]		    = [&](const auto a, const auto b)
						   {
								return std::filesystem::last_write_time(a->location) > 
									   std::filesystem::last_write_time(b->location);
						   };


	lamMap["Average Tag Score"] = [&](const auto a, const auto b)
							{
								const auto aTags = collection.booruTags.getTags(*a);
								const auto bTags = collection.booruTags.getTags(*b);

								const auto getScores	= [&](const auto& tags)
								{
									std::map< cute::Tag, cute::SkillDatum> acc;
									std::vector< std::tuple<cute::Tag, cute::SkillDatum>> out;

									for(const auto t : tags)
									{
										const auto it = acc.find(t); 
										if(it != acc.end()) continue;

										if(t.tag == "no_sauce") continue;
										if(t.tag == "no_gelbooru") continue; 
										if(t.tag == "no_danbooru") continue;

										acc.insert( std::make_pair( t, collection.booruTags.scores.retrieveData(t)));
									}


									for(const auto p : acc)
									{
										out.push_back( std::make_tuple( p.first, p.second));
									}

									return out;
								};


								const auto averageSkill = [&](const auto& pairs)
								{
									int num = 0;
									int askill = 0;
									for(const auto [tag, skill] : pairs)
									{
										askill += skill.skill();
										num++;
									}

									if(num == 0) return 0;
									return askill / num;
								};

								const auto aCollect = getScores(aTags);
								const auto bCollect = getScores(bTags);

								const auto aAverage = averageSkill(aCollect);
								const auto bAverage = averageSkill(bCollect);

								if(aAverage == bAverage)
								return collection.identityRanker.getSkill(*a).skill() > 
									   collection.identityRanker.getSkill(*b).skill();


								return aAverage > bAverage; 
							};

	lamMap["Character"]	= [&](const auto a, const auto b)
								{
								const auto aTags = collection.charTags.getTags(*a);
								const auto bTags = collection.charTags.getTags(*b);

								const auto getScores	= [&](const auto& tags)
								{
									std::map< cute::Tag, cute::SkillDatum> acc;
									std::vector< std::tuple<cute::Tag, cute::SkillDatum>> out;

									for(const auto t : tags)
									{
										const auto it = acc.find(t); 
										if(it != acc.end()) continue;
										acc.insert( std::make_pair( t, collection.charTags.scores.retrieveData(t)));
									}


									for(const auto p : acc)
									{
										out.push_back( std::make_tuple( p.first, p.second));
									}

									return out;
								};


								const auto averageSkill = [&](const auto& pairs)
								{
									int num = 0;
									int askill = 0;
									for(const auto [tag, skill] : pairs)
									{
										askill += skill.skill();
										num++;
									}

									if(num == 0) return 0;
									return askill / num;
								};

								const auto aCollect = getScores(aTags);
								const auto bCollect = getScores(bTags);

								const auto aAverage = averageSkill(aCollect);
								const auto bAverage = averageSkill(bCollect);

								if(aAverage == bAverage)
								return collection.identityRanker.getSkill(*a).skill() > 
									   collection.identityRanker.getSkill(*b).skill();

								return aAverage > bAverage; 
							};



	Gtk::ComboBoxText* combo;
	builder->get_widget("sortBox", combo);
	const std::string boxStr = combo->get_active_text().raw();

	std::sort(images.begin(), images.end(), lamMap[boxStr]);
}

//callback for the right click 'info' menu
void BrowseWindow :: rightClick (const std::vector<Gtk::TreeModel::Path> paths)
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

//callback for the right click 'info' menu
void BrowseWindow :: exileClick (const std::vector<Gtk::TreeModel::Path> paths)
{
	/*
	for(const auto p : paths)
	{
		auto iter = m_refTreeModel->get_iter(p);
		auto row = *iter; 
		if(!iter) 
		{
			std::cout << "REE" << std::endl;
			return;
		}

		std::shared_ptr<cute::Image>  image = row[m_Columns.m_col_image];
		auto path = image->location;
		auto c = std::filesystem::current_path();

		std::cout << "remove collection" << std::endl;
		collection.remove(image);

		std::cout << "remove model" << std::endl;
		m_refTreeModel->erase(iter);

		std::cout << "remove path" << std::endl;
		std::filesystem::remove(path);
	}
	*/
}

//get a pointer to our window
Gtk::Window* BrowseWindow :: getWindow (void)
{
	return window; 
}

//callback for when we select an image, opens the fight window
void BrowseWindow :: openFightWindow (const Gtk::TreeModel::Path& path)
{
	auto iter = m_refTreeModel->get_iter(path);
	auto row = *iter; 

	std::shared_ptr<cute::Image>  image = row[m_Columns.m_col_image];
	std::cout << image->location << '\n';

	collection.setLeftImage(image);
	fight.refresh();
	fight.getWindow()->show_all_children();
	fight.getWindow()->show();
}

//void BrowseWindow :: tagSelect (const Gtk::TreeModel::Path& path)

//importing every image in a directory
void BrowseWindow :: import_folder (void)
{
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

		  std::vector<cute::SharedImage> i = collection.getImages();



		  double num = 0;
		  double acc = 0;

			for(auto &f : std::filesystem::recursive_directory_iterator(dialog.get_filename(), std::filesystem::directory_options::follow_directory_symlink))
			{
				if(!cute::conformingFileType(f.path())) continue;
				i.push_back( std::make_shared<cute::Image> (f.path(), hash.retrieveData(f.path())));
				acc += collection.identityRanker.getSkill(*i.back()).sigma;
				num++;
			}

			std::cout << "average sigma" << acc / num << std::endl;
			collection.setImages(i);

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

void BrowseWindow :: filterByEntry (void)
{
	const std::string str = filterSearch->get_text().raw();

	collection.filter(str);

	importCollection();
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
	importCollection();
	refreshTagTree();

}

void BrowseWindow :: refresh (void)
{
	collection.filter();

	std::vector<cute::SharedImage> images = collection.getImages();
	comboSort(images);
	collection.setImages(images);
	importCollection(images);
	refreshTagTree();

}
}
