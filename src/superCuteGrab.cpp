#include <iostream>
#include "image.h"
#include "metaData.h"

int main(int argc, char *const argv[])
{
	if(argc != 2) {
		std::cout<< "Usage: " << argv[0] << " file\n";
		return 1;
	}
	//cute::Image test(argv[1]);
	cute::MetaData test(argv[1]);

	test.readTags();
	test.printTags();
	test.addTag("helloWorld");
	test.printTags();
	
	return 0;

}

