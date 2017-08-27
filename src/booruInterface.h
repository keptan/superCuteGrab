#ifndef BOORUITNERFACE_H
#define BOORUINTERFACE_H

#include <curl/curl.h>
#include <string>
#include <vector>
#include <json/json.h>
#include <mutex>


#include "metaData.h"
#include "image.h"


namespace cute
{
	class BooruInterface : public MetaData
	{
		protected :
			std::string url;
			std::string doc;
			long http_code;

			std::vector<std::string> docTags;

			static size_t handle(char * data, size_t size, size_t nmemb, void * p);
			size_t handle_impl(char * data, size_t size, size_t nmemb);

		public :
			
			BooruInterface(std::string m);

			bool getDoc();
			int readDocTags();
			bool reCurl();
			void  printDocTags();
			void printUrl();
			bool read();

			void writeDocTags();

			void printDoc();


			std::vector<std::string> getDocTags();

	};
};

#endif
			

