#include "image.h"

namespace cute {

PathMetaData :: PathMetaData (unsigned long fs, unsigned long wt, const PathMetaData::Hash& h)
	: file_size(fs), write_time(wt), hash(h) 
{}


Image :: Image (const std::filesystem::path l, const PathMetaData p)
	: location(l), pData(p)
{}

}
