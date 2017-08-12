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
       : path(p)
    {}

    fs::path Image :: fileName()
    {
        return path.filename();
    }

    fs::path Image :: filePath()
    {
        return path;
    }

	void Image :: getHash()
	{

		hash = hashFile(path.string().c_str());
		std::cout<<hash<<'\n';

	}

};
