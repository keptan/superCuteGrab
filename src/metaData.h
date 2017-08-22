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

			void printTags();
			int readTags();

			std::vector<std::string> getTags();

			void addTag(std::string);
			void clearTags();
			void writeTags();
			//int addTag(std::vector<std::string>);


	};

};

#endif



			
			


