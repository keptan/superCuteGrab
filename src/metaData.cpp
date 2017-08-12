#include "image.h"
#include "metaData.h"

#include <exiv2/exiv2.hpp>
#include <vector>
#include <string>

namespace cute
{
	MetaData :: MetaData(std::string p): Image(p)
	{

	}


	void MetaData :: readTags()
	{
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(Image::filePath());
		image.get();
		image->readMetadata();
		Exiv2::IptcData &iptcData = image->iptcData();

		Exiv2::IptcData::const_iterator end = iptcData.end();
		tags.clear();
		for (Exiv2::IptcData::const_iterator i = iptcData.begin(); i != end; ++i) 
			tags.push_back( i->toString());

	}


	void MetaData :: printTags()
	{
		for (auto & n : tags)
			std::cout << n << '\n';
	}

	void MetaData :: addTag(std::string s)
	{
		Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(Image::filePath());
		image.get();
		image->readMetadata();
		Exiv2::IptcData &iptcData = image->iptcData();


		Exiv2::Value::AutoPtr v = Exiv2::Value::create(Exiv2::string);

		v->read(s);

		iptcData.add(Exiv2::IptcKey("Iptc.Application2.Keywords") , v.get());

		image->setIptcData(iptcData);
		image->writeMetadata(); //replace with modifying tag list vector and write with seperate method
	}




};





