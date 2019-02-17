#include "booru.h"
#include "filetypes.h"
#include <iostream>
#include <sstream>

namespace cute {

DanBooru :: DanBooru (const std::string& hash)
	: url (std::string("https://danbooru.donmai.us/posts.json?tags=md5:") + hash)
{

}

size_t DanBooru :: handle(char * data, size_t size, size_t nmemb, void * p)
{
	return static_cast<DanBooru*>(p)->handle_impl(data, size, nmemb);
}

size_t DanBooru :: handle_impl(char* data, size_t size, size_t nmemb)
{
	doc.append(data, size * nmemb);
	return size * nmemb;
}

int DanBooru :: getDoc (void)
{
		CURL *curl;
		CURLcode res;
		struct curl_slist *headers=NULL;

		curl = curl_easy_init();


		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,&DanBooru::handle);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		/*
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,&BooruInterface::handle);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		http_code = 0;
		*/
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

int DanBooru :: genTags (void)
{
	if( getDoc()) return -1;
	if( parseDoc()) return -1;

	return 0;
}

Gelbooru :: Gelbooru (const std::string& hash)
	: main_url (std::string("https://gelbooru.com//index.php?page=dapi&s=post&q=index&json=1&limit=1&tags=md5:") + hash)
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


int Gelbooru :: parseDoc (std::string inDoc)
{
	Json::Reader reader;
	Json::Value  data;

	std::string tagString;

	if (inDoc == "[][]") return -1;
	if (inDoc == "[]")   return -1;
	if (inDoc == "")	   return -1;

	if(! reader.parse(inDoc, data)) return -1;


	tagString	 = data[0]["tags"].asString();

	std::vector<std::string> tagVec;

	std::string splitBuf;
	std::stringstream ss(tagString);

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

int Gelbooru :: genTags (void)
{
	if( getDoc(main_url)) return -1;
	if( parseDoc(doc)) return -1;

	return 0;
}

int booruScan (const std::filesystem::path loc, HashDB& hash, TagDB& general, TagDB& artists, TagDB& characters)
{
	hash.scanDirectory(loc);
	int tagged = 0;
	int files = 0;
	int alreadyTagged = 0;

	for(const auto it : hash)
	{
		files++;
		const auto h = it.second.hash;
		DanBooru booru(h);
		Gelbooru gbooru(h);

		const auto c = [&](const auto s){ return general.retrieveData(h).contains(Tag(s));};
		const auto checkedG = c("no_gelbooru");
		const auto hasG		= c("gelbooru");
		const auto checkedD = c("no_danbooru");
		const auto hasD		= c("danbooru");

		if(hasG || hasD || ( checkedG && checkedD))
		{

			std::cout << "already tagged: " << it.first.filename().string() << '\n';
			alreadyTagged++;
			continue;
		}

		const auto gotDanbooru = checkedD ? -1 : booru.genTags();

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

		const auto gotGelbooru = checkedG ? -1 : gbooru.genTags();

		if(!checkedG && !gotGelbooru)
		{
			std::cout << "searched gelbooru" << it.first.filename().string() << '\n';
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


	}

	std::cout << "scanned: " << files << " files\n";
	std::cout << alreadyTagged << " were already tagged\n";
	std::cout << tagged << " added tags\n";

	return 0;

}

							}
