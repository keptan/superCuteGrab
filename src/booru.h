#pragma once 

#include <curl/curl.h>
#include <json/json.h>
#include "tagSet.h"
#include "tagDB.h"
#include "hashDB.h"

#include <optional>
#include <filesystem>

namespace cute
{
	class DanBooru
	{ 
		using Hash = std::string;
		const std::string url;
		std::string doc;
		long http_code;



		int getDoc (void);
		int parseDoc (void);

		protected:
		static size_t handle (char*, size_t, size_t, void*);
		size_t handle_impl	 (char*, size_t, size_t);

		public:
		DanBooru (const Hash&);

		int genTags (void);
		TagSet artists;
		TagSet characters;
		TagSet general;
	};

	class Gelbooru
	{
		using Hash = std::string; 
		const std::string main_url;

		std::string doc;
		long http_code;
		int getDoc (const std::string&);
		int parseDoc (std::string);

		std::map<std::string, std::string> tagTypes;

		std::string getType (std::string);

		protected:
		static size_t handle (char*, size_t, size_t, void*);
		size_t handle_impl	 (char*, size_t, size_t);

		public:
		Gelbooru (const Hash&);

		int genTags (void);
		TagSet artists;
		TagSet characters;
		TagSet general;
	};


	int booruScan (const std::filesystem::path, HashDB&, TagDB&, TagDB&, TagDB&);
}


		
		

