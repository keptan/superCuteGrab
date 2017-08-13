#ifndef BOORUINTERFACE_H
#define BOORUINTERFACE_H

#include <curl/curl.h>
#include <string>
#include <vector>
#include <json/json.h>


namespace cute
{
	class BooruInterface
	{
		protected :
			std::string loc;
			std::string md5;
			std::string doc;

			std::vector<std::string> docTags;

			size_t curl_write (void *ptr, size_t size, size_t nmemb, void *stream);

		public :

			BooruInterface(std::string m);
			

			void getDoc();
			void readTags();
			void printTags();

			std::vector<std::string> getTags();
			
	};
};



#endif
