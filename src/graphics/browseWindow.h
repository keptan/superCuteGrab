#pragma once 
#include "infoPopup.h" 

namespace graphics 
{
class BrowseWindow : public sigc::trackable 
{

	FightWindow fight;
	private:
	const Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Window* window; 
	ImageIcons view;

	Gtk::Menu menuPopup;
	Gtk::Entry* filterSearch;
	Gtk::TreeView* tagTree;

	std::unique_ptr<InfoPopup> iPop;
	cute::ThumbDB thumbnails; 
	cute::CollectionMan& collection; 
	cute::HashDB& hash;

	std::map<cute::SharedImage, Glib::RefPtr<Gdk::Pixbuf>> pixCache;

	//threadpools
	Glib::RefPtr<Gdk::Pixbuf> defaultIcon;
	void addThumbs(void);
	void generateIcons (void);
	int iconContext_;

	std::mutex m;
	FutureDad cpuPool;
	FutureDad syncro;
	int context;
	std::optional<std::future<void>> get;
	void importCollection (void);
	void importCollection (const std::vector<cute::SharedImage>&);


	public:
	BrowseWindow( const Glib::RefPtr<Gtk::Builder>, cute::CollectionMan&, cute::HashDB&);
	~BrowseWindow (void);
	Gtk::Window* getWindow (void);

	protected:
	void openFightWindow (const Gtk::TreeModel::Path&);
	void addMember (const std::shared_ptr<cute::Image> i);
	void import_folder (void);
	void import_folder_recursive (void);
	void filterByEntry (void);
	void refreshTagTree (void);
	void filterTagTree (void);
	void refresh (void);
	void comboSort (std::vector<cute::SharedImage>&);

	void on_dropped_file(const Glib::RefPtr<Gdk::DragContext>& context,
	  int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time);

	void get_selected_data (
		   const Glib::RefPtr<Gdk::DragContext>& context, 
			Gtk::SelectionData& selection_data, guint info, guint time);	   

	void rightClick (const std::vector<Gtk::TreeModel::Path>);
	void exileClick (const std::vector<Gtk::TreeModel::Path>);


	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	IconColumns m_Columns;

	Glib::RefPtr<Gtk::ListStore> tagTreeModel;
	TagColumns tagColumns;

};
}
