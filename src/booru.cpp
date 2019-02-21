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

SauceArbiter :: SauceArbiter (std::string key)
	: sm(key), thumbs("thumbnails")
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

			booru.idQ(y.danbooru_id);
			artists = booru.artists;
			characters = booru.characters;
			general = booru.general;

			return 0;
		}
		if(hasG)
		{
			if(y.gelbooru_id == "0") return -1;
			if(y.similarity < 80) continue;

			gbooru.idQ(y.gelbooru_id);
			artists = gbooru.artists;
			characters = gbooru.characters;
			general = gbooru.general;

			return 0;
		}
	}

	return -1;
}

void SauceArbiter :: clear (void)
{
	artists = TagSet();
	characters = TagSet();
	general = TagSet();
	booru.clear();
	gbooru.clear();
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



int booruScan (const std::filesystem::path loc, HashDB& hash, TagDB& general, TagDB& artists, TagDB& characters)
{
	hash.scanDirectoryRecursive(loc);
	int tagged = 0;
	int files = 0;
	int alreadyTagged = 0;

	std::ifstream t("saucenao_key");
	std::string key;
	t >> key;


	DanBooru booru;
	Gelbooru gbooru;
	SauceArbiter arbiter(key);

	for(const auto it : hash)
	{
		booru.clear();
		gbooru.clear();
		arbiter.clear();

		files++;
		const auto h = it.second.hash;
		const auto set = general.retrieveData(h);

		const auto checkedG = set.contains(Tag("no_gelbooru"));
		const auto hasG		= set.contains(Tag("gelbooru"));
		const auto checkedD = set.contains(Tag("no_danbooru"));
		const auto hasD		= set.contains(Tag("danbooru"));
		const auto checkedS = set.contains(Tag("no_sauce"));
		const auto checkedAll = (checkedG && checkedD) || checkedS;


		if(hasG || hasD || checkedAll)
		{

			std::cout << "already tagged: " << it.first.filename().string() << '\n';
			alreadyTagged++;
			continue;
		}

		const auto gotDanbooru = checkedD ? -1 : booru.hashQ(h);

		if(!checkedD && !gotDanbooru)
		{

			const TagSet t( Tag("no_danbooru"));
			general.insertTags(h, t);
		}

		if(!gotDanbooru)
		{
			std::cout << "found danbooru for: " << it.first.filename().string() << '\n';
			general.insertTags(h, booru.general);
			artists.insertTags(h, booru.artists);
			characters.insertTags(h, booru.characters);
			tagged++;
			continue;
		}

		const auto gotGelbooru = checkedG ? -1 : gbooru.hashQ(h);

		if(!checkedG && gotGelbooru)
		{
			const TagSet t( Tag("no_gelbooru"));
			general.insertTags(h, t);
		}

		if(!gotGelbooru)
		{
			std::cout << "found gelbooru for: " << it.first.filename().string() << '\n';
			general.insertTags(h, gbooru.general);
			artists.insertTags(h, gbooru.artists);
			characters.insertTags(h, gbooru.characters);
			tagged++;
			continue;
		}

		try
		{
			const auto gotSauce = arbiter.search(it.first, h);
			if(!gotSauce)
			{
				std::cout << "found saucenao for: " << it.first.filename().string() << '\n';
				general.insertTags(h, arbiter.general);
				artists.insertTags(h, arbiter.artists);
				characters.insertTags(h, arbiter.characters);
				tagged++;
				continue;
			}
			if(gotSauce)
			{
				const TagSet t( Tag("no_sauce"));
				general.insertTags(h, t);
			}
		}
		catch(const Gdk::PixbufError& e)
		{
			std::cout << "broken image: " << it.first.filename() << '\n';
			continue;
		}



	}

	std::cout << "scanned: " << files << " files\n";
	std::cout << alreadyTagged << " were already tagged\n";
	std::cout << tagged << " added tags\n";

	return 0;

}

							}
