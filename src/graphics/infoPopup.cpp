#include "infoPopup.h" 
#include <algorithm>
#include <glib.h>
#include <glib/gi18n.h>



namespace graphics
{

InfoPopup :: InfoPopup ( const Glib::RefPtr<Gtk::Builder> b 
					   , cute::ThumbDB& t, cute::CollectionMan& c)

   :   thumbnails(t), collection(c), builder(b)
   , infoImage(builder, "infoAspect", "infoScroll", "infoImage"), lastSize(false)
{



	//setup builder widgets
	builder->get_widget("infoWindow", window);
	window->resize(500,500);
	builder->get_widget("infoTags", tagTree);
	builder->get_widget("addTag", addTag);
	builder->get_widget("infoAspect", frame);
	builder->get_widget("infoIconScroll", scroll);

	builder->get_widget("character", character);
	builder->get_widget("artist", artist);
	builder->get_widget("viewCharacter", viewCharacter);
	builder->get_widget("viewArtist", viewArtist);

	//setup keypresses 
	window->signal_key_press_event().connect(sigc::mem_fun(*this, &InfoPopup::onKeyPress), false);

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

void InfoPopup :: setImages (const cute::SharedImage i)
{
	std::vector<cute::SharedImage> vec;
	vec.push_back(i);

	setImages(vec);
}

void InfoPopup :: setImages (const std::vector< cute::SharedImage> i)
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

	const auto allSortedTags = [&]()
	{
		auto tupleVec = collection.tagsWithScores(selected);
		std::sort(tupleVec.begin(), tupleVec.end(), 
				[](const auto a, const auto b)
				{
					return std::get<1>(a).skill() > std::get<1>(b).skill();
				});

		return tupleVec;
	}();


	tagTreeModel->clear();

	if(selected.size() == 1)
	{
		Gtk::TreeModel::Row r     = *(tagTreeModel->append());
		r[tagColumns.m_col_name]  = i[0]->location.filename().string();
		r[tagColumns.m_col_score] = collection.identityRanker.getSkill(*i[0]).skill();
	}

	for(const auto tuple : allSortedTags)
	{
		Gtk::TreeModel::Row r = *(tagTreeModel->append());
		r[tagColumns.m_col_name]		= std::get<0>(tuple).tag;
		r[tagColumns.m_col_score]	= std::get<1>(tuple).skill();
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
		scale_in_hell();
		return;
	}

	for(const auto im : i)
	{
		//append all the images into the iconView widget
		Gtk::TreeModel::Row r = *(iconTreeModel->append());



		r[iconColumns.m_col_name] = im->location.string();
		r[iconColumns.m_col_pixbuf] =  Gdk::Pixbuf::create_from_file( thumbnails.getThumbPath(*im).c_str());
		r[iconColumns.m_col_image] = im; 
	}




	scroll->show();
	infoImage.hide();
	icons.show();
}

void InfoPopup :: downScroll (void)
{
	if(selected.size() != 1) return;

	const auto images = collection.getImages();
	const auto it = std::find(images.begin(), images.end(), selected[0]);

	if( it == images.end()) return;
	const auto next = it + 1 == images.end() ? images.begin() : it + 1;
	setImages(*next);


}

void InfoPopup :: upScroll (void)
{
	if(selected.size() != 1) return;

	const auto images = collection.getImages();
	const auto it = std::find(images.begin(), images.end(), selected[0]);

	if( it == images.end()) return;
	const auto next = it == images.begin() ? images.end() -1 : it - 1;
	setImages(*next);

}

void InfoPopup :: scale_in_hell (void)
{

		if(lastSize)
		{
			infoImage.hide();
			window->resize( window->get_width() - 5, window->get_height() - 5);
			window->resize( window->get_width() + 1, window->get_height() + 1);
			infoImage.show();

		//	leftImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&leftImage, &Window::ScalingImage::scaleImage));
		//	rightImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&rightImage, &Window::ScalingImage::scaleImage));


			window->resize( window->get_width() - 5, window->get_height() - 5);
			window->resize( window->get_width() + 5, window->get_height() + 5);
		}
		else 
		{

		//	leftImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&leftImage, &Window::ScalingImage::scaleImage));
		//	rightImage.scrollView->signal_size_allocate().connect(sigc::mem_fun(&rightImage, &Window::ScalingImage::scaleImage));

			infoImage.hide();
			window->resize( window->get_width() + 5, window->get_height() + 5);
			window->resize( window->get_width() - 5, window->get_height() - 5);
			infoImage.show();
		}


		lastSize = !lastSize;
}

bool InfoPopup :: onKeyPress (GdkEventKey *event)
{
	if( event->hardware_keycode == 113) 
	{
		//left key 
		upScroll();	
		return true; 
	}

	if( event->hardware_keycode == 111)
	{
		//up key

		scale_in_hell();
		return true;
	}


	if( event->hardware_keycode == 114) 
	{
	
		//right key 
	
		downScroll();
		return true; 
	}


	if( event->hardware_keycode == 116) 
	{
		//downkey

		scale_in_hell();
		return true;
	}

	std::cout << event->hardware_keycode << '\n';

	return false;
}



}
