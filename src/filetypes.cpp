#include "filetypes.h" 

namespace cute {
std::set<std::string> extensions  = {".jpg", ".jpeg", ".png", ".JPEG"};

bool conformingFileType (const std::filesystem::path& p)
{
	return  extensions.find(p.extension()) != extensions.end();
}
}

