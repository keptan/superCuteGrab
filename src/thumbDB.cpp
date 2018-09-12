#include "thumbDB.h"
#include <iostream>

namespace cute {

ThumbDB :: ThumbDB (std::filesystem::path p)
	: thumbDir(p)
{
	assert( std::filesystem::is_directory(thumbDir));

	for(auto &f : std::filesystem::directory_iterator(thumbDir))
		items.insert( std::make_pair(f.path().filename(), std::filesystem::absolute(f.path())));
}

std::filesystem::path ThumbDB :: getThumbPath (const Image& image)
{
	const auto it = items.find(image.pData.hash);

	if(it != items.end()) return it->second;
	std::cout << "generating new thumbnail for: " << image.location << std::endl;

	const auto newPath = generateThumb(image);
	items.insert( std::make_pair(image.pData.hash, newPath));

	return newPath;
}




std::filesystem::path ThumbDB :: generateThumb (const Image& image)
{
	auto sourcePBuf = Gdk::Pixbuf::create_from_file(image.location.c_str());

	const int width = sourcePBuf->get_width();
	const int height = sourcePBuf->get_height(); 
	const float ratio = (float) width / (float) height; 

	const int outHeight = 100;
	const int outWidth = 100 / ratio; 

	auto scaledPBuf = sourcePBuf->scale_simple(
					outHeight, outWidth,
					Gdk::INTERP_BILINEAR);

	scaledPBuf->save( (thumbDir / image.pData.hash).c_str(), "jpeg");

	return std::filesystem::absolute(thumbDir/ image.pData.hash);
}

	

}

