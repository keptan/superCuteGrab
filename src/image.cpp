#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <experimental/filesystem>
#include "image.h"
#include "fileMd5.h"

namespace fs = std::experimental::filesystem;
namespace cute
{
    Image :: Image(std::string p)
       : path(p),hash("")
    {}

    fs::path Image :: fileName()
    {
        return path.filename();
    }

    fs::path Image :: filePath()
    {
        return path;
    }

	std::string Image :: makeHash()
	{
		hash = hashFile(path.string().c_str());
		return hash;
	}

	std::string Image :: getHash()
	{
		if(hash.length() == 0)
			return makeHash();

		return hash;

	}

};
