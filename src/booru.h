#pragma once 

#include <curl/curl.h>
#include <json/json.h>
#include "tagSet.h"
#include "tagDB.h"
#include "hashDB.h"
#include "../sauce/sauce.hpp"
#include "thumbDB.h"

#include <optional>
#include <filesystem>

namespace cute
{
	class DanBooru
	{ 
		using Hash = std::string;
		std::string doc;
		long http_code;



		int getDoc (const std::string&);
		int parseDoc (void);

		protected:
		static size_t handle (char*, size_t, size_t, void*);
		size_t handle_impl	 (char*, size_t, size_t);

		public:
		DanBooru (void);

		int hashQ (const Hash&);
		int idQ	  (const std::string&);

		TagSet artists;
		TagSet characters;
		TagSet general;
	};

	class Gelbooru
	{
		using Hash = std::string; 

		std::string doc;
		long http_code;

		int getDoc (const std::string&);
		int parseDoc (void);

		std::map<std::string, std::string> tagTypes;
		std::string getType (std::string);

		protected:
		static size_t handle (char*, size_t, size_t, void*);
		size_t handle_impl	 (char*, size_t, size_t);

		public:
		Gelbooru (void);

		int hashQ (const Hash&);
		int idQ	  (const std::string&);

		TagSet artists;
		TagSet characters;
		TagSet general;
	};

	class SauceArbiter
	{
		using Hash = std::string;
		sauce::sauceMech sm;
		ThumbDB thumbs;
		DanBooru booru;
		Gelbooru gbooru;

		public:
		SauceArbiter (std::string);

		int search (const std::filesystem::path& p, const Hash& h);

		TagSet artists;
		TagSet characters;
		TagSet general;

	};




	int booruScan (const std::filesystem::path, HashDB&, TagDB&, TagDB&, TagDB&);
}


		
		


