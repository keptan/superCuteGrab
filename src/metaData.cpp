#include "image.h"
#include "metaData.h"

#include <exiv2/exiv2.hpp>
#include <vector>
#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#include <time.h>

namespace cute
{

	MetaData :: MetaData(std::string p)
		: Image(p)
	{}

	int MetaData :: readTags()
	{
		try {
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(Image::filePath());
		image.get();
		image->readMetadata();
		Exiv2::IptcData &iptcData = image->iptcData();

		Exiv2::IptcData::const_iterator end = iptcData.end();
		tags.clear();
		for (Exiv2::IptcData::const_iterator i = iptcData.begin(); i != end; ++i){
			tags.push_back( i->toString());

		}

		image->io().close();
		return 0;
		}
		catch(Exiv2::AnyError& e)
		{
			return 1;
		}

	}

	void MetaData :: printTags()
	{
		for (auto & n : tags)
			std::cout << n << '\n';
	}

	std::string MetaData :: tagsString()
	{
		std::string output;

		for (auto &n : tags){
			output += n;
			output += ' ';
			
		}

		return output;
	}

	void MetaData :: addTag(std::string s)
	{
	
		tags.push_back(s);

	}

	void MetaData :: clearTags()
	{
		tags.clear();
	}

	void MetaData :: writeTags()
	{	
		time_t mtime;
		struct stat attrib;
		struct utimbuf new_times;

		mtime = attrib.st_mtime;

		new_times.modtime = attrib.st_mtime;
		new_times.actime = time(NULL);

		

		stat(Image::filePath().string().c_str(),&attrib);


		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(Image::filePath());
		image.get();
		image->readMetadata();
		Exiv2::IptcData iptcData;


		Exiv2::Value::AutoPtr v = Exiv2::Value::create(Exiv2::string);

		for(auto element : tags){
		v->read(element);

		iptcData.add(Exiv2::IptcKey("Iptc.Application2.Keywords") , v.get());
		}

		image->setIptcData(iptcData);
		image->writeMetadata(); //replace with modifying tag list vector and write with seperate method

		utime(Image::filePath().string().c_str(),&new_times);


		

	}

	bool MetaData :: hasTag(std::string s)
	{
		if (tags.empty())
			return false;

		if (std::find(tags.begin(), tags.end(), s) != tags.end())
			return true;

		for(auto &t : tags){
			if(s.compare(0,s.size(),t))
				return true;
		}
	}

	bool MetaData :: tagged()
	{
		std::string s = "MD5:";
	
		if (tags.empty())
			return false;

		for(auto &t : tags)
			if(s == t.substr(0,s.size()))
				return true;
	

			return false;

		}

	std::vector<std::string> MetaData :: getTags()
	{
		return tags;
	}

};





