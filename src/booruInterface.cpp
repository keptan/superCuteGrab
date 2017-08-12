#include <curl/curl.h>
#include <string>
#include <vector>
#include "tinyxml.h"

namespace cute
{
	BooruInterface :: BooruInterface(std::string l,std::string p) 
		: loc(l)
	{}

	void BooruInterface :: getDoc()
	{
		CURL *curl;
		CURLcode res;

		curl_global_init(CURL_GLOBAL_ALL);

		curl = curl_easy_init();
		
		if(curl) {
			curl_easy_setopt(curl,CURLOPT_URL,loc);

			curl_easy_setopt(curl,CURLOPT_POSTFIELDS,post);


