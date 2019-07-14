#include "booru.h"
#include "filetypes.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <sstream>

namespace cute {



DanBooru :: DanBooru (void)
{}

size_t DanBooru :: handle(char * data, size_t size, size_t nmemb, void * p)
{
	return static_cast<DanBooru*>(p)->handle_impl(data, size, nmemb);
}

size_t DanBooru :: handle_impl(char* data, size_t size, size_t nmemb)
{
	doc.append(data, size * nmemb);
	return size * nmemb;
}

int DanBooru :: getDoc (const std::string& url)
{
		CURL *curl;
		CURLcode res;
		struct curl_slist *headers=NULL;

		curl = curl_easy_init();


		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,&DanBooru::handle);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		return 0;
}

int DanBooru :: parseDoc (void)
{
	Json::Reader reader;
	Json::Value  data;

	std::string tagString;
	std::string charString;
	std::string artistString;


	if (doc == "[][]") return -1;
	if (doc == "[]")   return -1;
	if (doc == "")	   return -1;

	if(! reader.parse(doc, data)) return -1;
	tagString	 = data[0]["tag_string_general"].asString();
	charString	 = data[0]["tag_string_character"].asString();
	artistString = data[0]["tag_string_artist"].asString();

	const auto toSet = [](const auto s, auto& tags)
	{
		std::string splitBuf;
		std::stringstream ss(s);

		while(ss >> splitBuf) tags += splitBuf;
	};

	toSet(tagString, general);
	toSet(charString, characters);
	toSet(artistString, artists);

	general += Tag("danbooru");

	return 0;

}

int DanBooru :: hashQ (const Hash& h)
{
	const std::string request = "https://danbooru.donmai.us/posts.json?tags=md5:";
	if(getDoc(request + h)) return -1;
	return parseDoc();
}

int DanBooru :: idQ (const std::string& id)
{
	const std::string request = "https://danbooru.donmai.us/posts.json?tags=id:";
	if(getDoc(request + id)) return -1;
	return parseDoc();
}

void DanBooru :: clear (void)
{
	artists = TagSet();
	characters = TagSet();
	general = TagSet();

	doc = "";
}


Gelbooru :: Gelbooru (void)
{
}

size_t Gelbooru :: handle(char * data, size_t size, size_t nmemb, void * p)
{
	return static_cast<Gelbooru*>(p)->handle_impl(data, size, nmemb);
}

size_t Gelbooru :: handle_impl(char* data, size_t size, size_t nmemb)
{
	doc.append(data, size * nmemb);
	return size * nmemb;
}

int Gelbooru :: getDoc (const std::string& url)
{
		CURL *curl;
		CURLcode res;
		struct curl_slist *headers=NULL;
		doc="";

		curl = curl_easy_init();


		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,&Gelbooru::handle);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
			
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		return 0;
}

std::string Gelbooru :: getType (std::string tag)
{
	const auto it = tagTypes.find(tag);
	if(it != tagTypes.end()) return it->second; 


	getDoc(std::string("https://gelbooru.com/index.php?page=dapi&s=tag&q=index&json=true&name=") + tag);

	Json::Reader reader;
	Json::Value  data;
	reader.parse(doc, data);

	std::string type = data[0]["type"].asString();
	tagTypes.insert( std::make_pair(tag, type));

	return type;
}

int Gelbooru :: parseDoc ()
{
	Json::Reader reader;
	Json::Value  data;

	std::string tagString;

	if (doc == "[][]") return -1;
	if (doc == "[]")   return -1;
	if (doc == "")	   return -1;

	if(! reader.parse(doc, data)) return -1;


	tagString	 = data[0]["tags"].asString();

	std::vector<std::string> tagVec;

	std::string splitBuf;
	std::stringstream ss(tagString);

	general += TagSet( std::string("gelbooru"));

	while(ss >> splitBuf) tagVec.push_back(splitBuf);

	for(const auto s : tagVec)
	{
		const auto type = getType(s);
		if(type == "tag")
		{
			general += TagSet(s);
		}
		if(type == "artist")
		{
			artists += TagSet(s);
		}
		if(type == "character")
		{
			characters += TagSet(s);
		}
	}


	return 0;
}

int Gelbooru :: hashQ (const Hash& h)
{

	const std::string request = "https://gelbooru.com//index.php?page=dapi&s=post&q=index&json=1&limit=1&tags=md5:";
	if(getDoc(request + h)) return -1;
	return parseDoc();
}

int Gelbooru :: idQ (const std::string& id)
{

	const std::string request = "https://gelbooru.com//index.php?page=dapi&s=post&q=index&json=1&limit=1&tags=id:";
	if(getDoc(request + id)) return -1;
	return parseDoc();
}

void Gelbooru :: clear (void)
{
	artists = TagSet();
	characters = TagSet();
	general = TagSet();

	doc = "";
}

SauceArbiter :: SauceArbiter (const std::string& key, ThumbDB& t)
	: sm(key), thumbs(t)
{}

int SauceArbiter :: search (const std::filesystem::path& p, const Hash& h)
{
	sm.set_image_path( thumbs.getThumbPath(h ,p));
	sm.fetch_json();

	auto x = sm.get_sauce_res();

	for(auto it = x.begin();it!=x.end();++it)
	{
		auto y = *it;
		bool hasD = (y.danbooru_tag_set.size());
		bool hasG = (y.gelbooru_tag_set.size());

		if(hasD)
		{
			if(y.danbooru_id == "0") return -1;
			if(y.similarity < 80) continue;
			dbooruID = y.danbooru_id;

			return 0;
		}
		if(hasG)
		{
			if(y.gelbooru_id == "0") return -1;
			if(y.similarity < 80) continue;
			gbooruID = y.gelbooru_id;

			return 0;
		}
	}

	return -1;
}

std::optional< std::string> SauceArbiter :: getDanbooru (void)
{

	return dbooruID.length()  ? std::optional<std::string>(dbooruID) : std::nullopt;
}
std::optional< std::string> SauceArbiter :: getGelbooru (void)
{
	return gbooruID.length()  ? std::optional<std::string>(gbooruID) : std::nullopt;
}

AsyncScanner :: AsyncScanner
(const std::filesystem::path p, HashDB& h, ThumbDB& t, TagDB& g, TagDB& c, TagDB& a)
: path(p), sauceLimit(15, 30000), syncro(1), ioPool(25), cpuPool(8)
, hash(h), thumbs(t), general(g), characters(c), artists(a)
{
	std::ifstream file("saucenao_key");
	file >> key;
}

AsyncScanner :: ~AsyncScanner (void)
{
	ioPool.join_finish();
	cpuPool.join_finish();
	syncro.join_finish();
}

void AsyncScanner :: perFile (const std::filesystem::path p, const std::string h)
{
	DanBooru db;
	Gelbooru gb;
	SauceArbiter sa(key, thumbs);



	const auto dscan = db.hashQ(h);
	if(!dscan)
	{
		syncro.addTask([=](){ std::cout << "found danbooru for:" << p.filename().string() << '\n';});
		std::scoped_lock lk (m);

		general.insertTags(h, db.general);
		artists.insertTags(h, db.artists);
		characters.insertTags(h, db.characters);
		return;
	}

	sauceLimit.waitAndUse();

	sa.search(p, h);
	const auto dSauce = sa.getDanbooru();
	const auto gSauce = sa.getGelbooru();

	if(dSauce != std::nullopt)
	{
		db.clear();
		const auto discan = db.idQ(*dSauce);
		syncro.addTask([=](){ std::cout << "sacenao found danbooru for:" << p.filename().string() << '\n';});
		std::scoped_lock lk (m);

		general.insertTags(h, db.general);
		artists.insertTags(h, db.artists);
		characters.insertTags(h, db.characters);
		return;
	}

	if(gSauce != std::nullopt)
	{
		gb.clear();
		const auto discan = gb.idQ(*gSauce);
		syncro.addTask([=](){ std::cout << "saucenao found gelbooru for:" << p.filename().string() << '\n';});
		std::scoped_lock lk (m);

		general.insertTags(h, gb.general);
		artists.insertTags(h, gb.artists);
		characters.insertTags(h, gb.characters);
		return;
	}

	syncro.addTask([=](){ std::cout << "saucenao couldn't find" << p.filename().string() << '\n';});
	std::scoped_lock lk(m);
	general.insertTags(h, TagSet(Tag("no_sauce")));

	return;
}

void AsyncScanner :: scan (void)
{
	hash.scanDirectoryRecursive(path);
	for(const auto it : hash)
	{
		const auto set = general.retrieveData(it.second.hash);
		const auto checkedS = set.contains(Tag("no_sauce"));
		const auto hasD		= set.contains(Tag("danbooru"));
		const auto hasG		= set.contains(Tag("gelbooru"));
		if(checkedS || hasD || hasG) continue;

		const auto f = [=](){ perFile(it.first, it.second.hash);};
		ioPool.addTask(f);
	}
	return;
}







int booruClean (const std::filesystem::path loc, HashDB& hash, TagDB& general, TagDB& artists, TagDB& characters)
{
	int files = 0;

	hash.scanDirectoryRecursive(loc);
	for(const auto it : hash)
	{
		files++;

		const auto h = it.second.hash;
		general.clearTags(h);
		artists.clearTags(h);
		characters.clearTags(h);
	}

	std::cout << "cleared tags from: " << files << " files\n";
	return 0;
}


							}
