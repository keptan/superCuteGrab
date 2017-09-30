#ifndef METADATA_H
#define METADATA_H

#include <exiv2/exiv2.hpp>
#include "image.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>



namespace cute
{
	class MetaData : public Image
	{

		protected :
			std::vector<std::string>tags;
			struct EqIgnoreCase;
			
		public :

			MetaData(std::string);
			bool hasTag(std::string);
			bool tagged();
			bool read = false;

			void printTags();
			int readTags();
			//int tempMu;

			std::vector<std::string> getTags();


			std::string tagsString();
			void addTag(std::string);
			void clearTags();
			void writeTags();
			void removeDup();
			
			//int addTag(std::vector<std::string>);


	};

};

#endif



			
			


