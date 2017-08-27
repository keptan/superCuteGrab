#ifndef IMAGE_H
#define IMAGE_H

#include <experimental/filesystem>
#include <string>

namespace fs = std::experimental::filesystem;
namespace cute
{
    class Image
    {
        protected :
            fs::path path;
            
        public :
			

			std::string hash;
            Image(std::string p);
            fs::path fileName();
            fs::path filePath();
			std::string getHash();
			std::string makeHash();

		

    };

};
#endif
