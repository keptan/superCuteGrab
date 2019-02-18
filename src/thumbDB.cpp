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
	return getThumbPath(image.pData.hash, image.location);
}


std::filesystem::path ThumbDB :: getThumbPath (const Hash& hash, const std::filesystem::path& path)
{

	const auto it = items.find(hash);

	if(it != items.end()) return it->second;
	const auto newPath = generateThumb(hash, path);
	items.insert( std::make_pair(hash, newPath));

	return newPath;
}


std::filesystem::path ThumbDB :: generateThumb (const Image& image)
{
	return generateThumb(image.pData.hash, image.location);
}

std::filesystem::path ThumbDB :: generateThumb (const Hash& hash, const std::filesystem::path& path)
{
	auto sourcePBuf = Gdk::Pixbuf::create_from_file(path.c_str());

	const int width = sourcePBuf->get_width();
	const int height = sourcePBuf->get_height(); 
	const float ratio = (float) width / (float) height; 

	const int outHeight = 190;
	const int outWidth = 190 / ratio; 

	auto scaledPBuf = sourcePBuf->scale_simple(
					outHeight, outWidth,
					Gdk::INTERP_BILINEAR);

	scaledPBuf->save( (thumbDir / hash).c_str(), "jpeg");

	return std::filesystem::absolute(thumbDir/hash);
}

	

}

