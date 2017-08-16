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

			std::vector<std::string> docTags;

			static size_t handle(char * data, size_t size, size_t nmemb, void * p);
			size_t handle_impl(char * data, size_t size, size_t nmemb);

		public :
			
			BooruInterface(std::string m);
			std::mutex isGetting;

			bool getDoc();
			bool readDocTags();
			void  printDocTags();

			void writeDocTags();


			std::vector<std::string> getDocTags();

	};
};

#endif
			

