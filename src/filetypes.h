#pragma once 
#include <set>
#include <string>
#include <filesystem>

namespace cute{

extern std::set<std::string> extensions ;
bool conformingFileType (const std::filesystem::path& p);

}
