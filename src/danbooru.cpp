#include "danbooru.h"
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

int booruScan (const std::filesystem::path loc, HashDB& hash, TagDB& general, TagDB& artists, TagDB& characters)
{
	hash.scanDirectory(loc);
	int tagged = 0;
	int files = 0;
	int alreadyTagged = 0;

	for(const auto &f : std::filesystem::recursive_directory_iterator(loc))
	{
		files++;
		if(!cute::conformingFileType(f.path())) continue;
		const auto h = hash.retrieveData(f.path()).hash;

		DanBooru booru(h);

		if(general.retrieveData(h).contains(Tag("danbooru")) || 
		   general.retrieveData(h).contains(Tag("no_danbooru"))) 
		{

			std::cout << "already tagged: " << f.path().filename().string() << '\n';
			alreadyTagged++;
			continue;
		}

		std::cout << "searching danbooru for: " << f.path().filename().string() << '\n';
		if(booru.genTags()) 
		{

			const TagSet t( Tag("no_danbooru"));
			general.insertTags(h, t);
			continue;
		}

		general.insertTags(h, booru.general);
		artists.insertTags(h, booru.artists);
		characters.insertTags(h, booru.characters);
		tagged++;
	}

	std::cout << "scanned: " << files << " files\n";
	std::cout << alreadyTagged << " were already tagged\n";
	std::cout << tagged << " added tags\n";

	return 0;

}

							}
