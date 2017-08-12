#ifndef METADATA_H
#define METADATA_H

#include <exiv2/exiv2.hpp>
#include "image.h"

#include <vector>
#include <string>

namespace cute
{
	class MetaData : public Image
	{
		protected :

			int writeToFile();

		protected :
			std::vector<std::string>tags;
			struct EqIgnoreCase;
			
		public :

			MetaData(std::string);
			bool hasTags;
			bool hasTag(std::string);
			void printTags();
			void readTags();

			std::vector<std::string> getTags();

			void addTag(std::string);
			//int addTag(std::vector<std::string>);


	};

};

#endif



			
			


