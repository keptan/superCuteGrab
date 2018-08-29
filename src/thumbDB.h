#pragma once 

#include <filesystem>
#include <string>
#include <gtkmm.h>
#include <set>
#include <cassert>
#include "hashDB.h"

namespace cute 
{
	class ThumbDB
	{
		public:
		using Hash = PathMetaData::Hash; 
		ThumbDB (std::filesystem::path);

		std::filesystem::path getThumbPath (const Image&);


		private:
		const std::filesystem::path thumbDir;
		std::map<Hash, std::filesystem::path> items; 

		std::filesystem::path generateThumb (const Image&);
	};
}

