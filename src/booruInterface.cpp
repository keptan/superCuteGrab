#include <curl/curl.h>
#include <string>
#include <sstream>
#include <vector>
#include "booruInterface.h"
#include "metaData.h"
#include "image.h"
#include <json/json.h>
#include <iostream>

namespace cute
{
	BooruInterface :: BooruInterface(std::string m)
		: MetaData(m),doc("")
	{}

	size_t BooruInterface :: handle(char * data, size_t size, size_t nmemb, void * p)
	{
		return static_cast<BooruInterface*>(p)->handle_impl(data, size, nmemb);
	}

	size_t BooruInterface :: handle_impl(char* data, size_t size, size_t nmemb)
	{
		doc.append(data, size * nmemb);
		return size * nmemb;
	}

	bool BooruInterface :: getDoc()
	{
		CURL *curl;
		struct curl_slist *headers=NULL;

		curl_slist_append(headers, "Accept: application/json");
		curl_slist_append(headers, "Content-Type: application/json");
		curl_slist_append(headers, "charset: utf-8");

		curl = curl_easy_init();

		url = "http://danbooru.donmai.us/posts.json?tags=md5:";
		url.append(Image::getHash());

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,&BooruInterface::handle);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	bool BooruInterface :: readDocTags()
	{
		Json::Value jsonData;
		Json::Reader jsonReader;

		std::string tagString;

		if (jsonReader.parse(doc,jsonData))
		{
			if (!jsonData[0].isMember("tag_string")){
				return false;
			}

			tagString = jsonData[0]["tag_string"].asString();
		}
		else
		{
			return false;
		}

		std::string splitBuf;
		std::stringstream ss(tagString);

		while(ss >> splitBuf)
			docTags.push_back(splitBuf);



		return true;

	}

	std::vector<std::string>  BooruInterface :: getDocTags()
	{
		return docTags;
	}

	void BooruInterface :: writeDocTags()
	{
		MetaData::addTag("MD5"+Image::getHash());
		for(auto s : docTags)
			MetaData::addTag(s);

		MetaData::writeTags();

		}




	void BooruInterface :: printDocTags()
	{
		int n = 0;
		for(auto i : docTags){
			std::cout<<i<<' ';
			n++;
			if (n>4)
				break;
		}

		
	}	

};

