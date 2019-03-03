#pragma once 

#include <curl/curl.h>
#include <json/json.h>
#include "tagSet.h"
#include "tagDB.h"
#include "hashDB.h"
#include "../sauce/sauce.hpp"
#include "thumbDB.h"

#include "../lib/nfun/future_pool.h"
#include "../lib/nfun/rate_limit.h"

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

		void clear (void);
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
		
		void clear (void);
	};

	class SauceArbiter
	{
		using Hash = std::string;
		sauce::sauceMech sm;

		ThumbDB& thumbs;

		std::string dbooruID;
		std::string gbooruID;

		public:
		SauceArbiter (const std::string&, ThumbDB&);

		int search (const std::filesystem::path& p, const Hash& h);

		std::optional< std::string> getDanbooru (void);
		std::optional< std::string> getGelbooru (void);
	};

	class AsyncScanner
	{
		std::mutex m;
		const std::filesystem::path path;
		std::string key;

		RateLimiter sauceLimit;
		FutureDad syncro;
		FutureDad ioPool;
		FutureDad cpuPool;

		HashDB& hash;
		ThumbDB& thumbs;
		TagDB&  general;
		TagDB&  characters;;
		TagDB&  artists;

		void perFile (const std::filesystem::path p, const std::string hash);

		public:
		AsyncScanner 
		(const std::filesystem::path, HashDB&, ThumbDB&, TagDB&, TagDB&, TagDB&);
		~AsyncScanner (void);

		void scan (void);
	};

	int booruScan (const std::filesystem::path, HashDB&, TagDB&, TagDB&, TagDB&);
	int booruClean (const std::filesystem::path, HashDB&, TagDB&, TagDB&, TagDB&);
}


		
		


