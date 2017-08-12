#ifndef METADATA_H
#define METADATA_h

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
			
		public :

			MetaData(std::string p);
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



			
			


