#ifndef BOORUINTERFACE_H
#define BOORUINTERFACE_H

#include <curl/curl.h>
#include <string>
#include <vector>
#include "tinyxml.h"


namespace cute
{
	class BooruInterface
	{
		protected :
			std::string loc;
			std::string post;
			TixmlDocument doc;
			std::vector<std::string> docTags;
			
		public :

			BooruInterface(std::string)
			

			void getDoc();

			void readTags();
			void printTags();

			std::string getMd5();
			std::vector<std::string> getTags();
			
	};
};



#endif
