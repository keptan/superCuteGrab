#include <curl/curl.h>
#include <string>
#include <sstream>
#include <vector>
#include "booruInterface.h"
#include "metaData.h"
#include "image.h"
#include <json/json.h>
#include <iostream>
#include <mutex>


namespace cute
{
	BooruInterface :: BooruInterface(std::string m)
		: MetaData(m),doc("")
	{}

	BooruInterface :: BooruInterface(MetaData  m)
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
		CURLcode res;
		struct curl_slist *headers=NULL;

		//doc = "";

		/*
		curl_slist_append(headers, "Accept: application/json");
		curl_slist_append(headers, "Content-Type: application/json");
		curl_slist_append(headers, "charset: utf-8");

		*/
		curl = curl_easy_init();

		url = "http://danbooru.donmai.us/posts.json?tags=md5:";
		url.append(Image::getHash());




		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl,CURLOPT_NOSIGNAL,1);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,&BooruInterface::handle);
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
		return true;
	}


	int BooruInterface :: readDocTags()
	{
		Json::Value jsonData;
		Json::Reader jsonReader;

		std::string tagString;

		if (doc == "[][]")
			return -1;

		if( doc == "[]")
			return 1;

		if(doc == "")
			return -1;

		if (jsonReader.parse(doc,jsonData))

		{
			if (!jsonData[0].isMember("tag_string"))
					return -1;

			tagString = jsonData[0]["tag_string"].asString();
		}
		else
		{
					return -1;
		}

		std::string splitBuf;
		std::stringstream ss(tagString);

		while(ss >> splitBuf)
			docTags.push_back(splitBuf);



		return 0;

	}

	bool BooruInterface :: reCurl()
	{
		if (doc == "[][]")
			return true;

		if (doc == "[]")
			return true;

		if (doc == "")
			return true;

	}

	bool BooruInterface :: read()
	{
		Json::Value jsonData;
		Json::Reader jsonReader;

		std::string tagString;

		if (doc == "[][]")
			return false;

		if( doc == "[]")
			return false;

		if(doc == "")
			return false;

		if (jsonReader.parse(doc,jsonData))

		{
			if (!jsonData[0].isMember("tag_string"))
				return false;

			return true;
		}
		else
		{
			return false;
		}


	}



	std::vector<std::string>  BooruInterface :: getDocTags()
	{
		return docTags;
	}

	void BooruInterface :: writeDocTags()
	{
		MetaData::addTag("MD5:"+Image::getHash());
		MetaData::addTag("booruTags");
		std::string parentName = Image::filePath().parent_path().filename().string();
		std::replace(parentName.begin(),parentName.end(),' ','_');
		parentName.insert(0,"folder:");
		MetaData::addTag(parentName);
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

	void BooruInterface :: printDoc()
	{
		std::cout<<"\nDOC START" << Image::fileName() <<" \n";
		std::cout<<http_code<<'\n';
		std::cout<<doc;
		std::cout<<"\nDOC END";
	}

	void BooruInterface :: printUrl()
	{
		std::cout<<url<<std::endl;
	}

};

