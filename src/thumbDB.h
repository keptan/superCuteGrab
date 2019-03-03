#pragma once 

#include <filesystem>
#include <string>
#include <gtkmm.h>
#include <set>
#include <cassert>
#include <mutex>

#include "hashDB.h"
#include "thumbDB.h"

namespace cute 
{
	class ThumbDB
	{
		public:
		using Hash = PathMetaData::Hash; 
		ThumbDB (std::filesystem::path);

		std::filesystem::path getThumbPath (const Image&);
		std::filesystem::path getThumbPath (const Hash&, const std::filesystem::path&);


		private:
		mutable std::mutex mutex;

		const std::filesystem::path thumbDir;
		std::map<Hash, std::filesystem::path> items; 

		std::filesystem::path generateThumb (const Image&);
		std::filesystem::path generateThumb (const Hash&, const std::filesystem::path&);

	};
}

