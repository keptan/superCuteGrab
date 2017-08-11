#include <string>
#include <cstddef>
#include <experimental/filesystem>
#include "image.h"

namespace fs = std::experimental::filesystem;
namespace cute
{
	Image :: Image(){}
    Image :: Image(std::string p)
       : path(p)
    {}

    fs::path Image :: fileName()
    {
        return path.filename();
    }

    fs::path Image ::filePath()
    {
        return path;
    }

};
