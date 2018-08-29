#include <iostream>
#include "hashDB.h"


int main(int argc, char *const argv[])
{
//	booruWriteScan(argv[1]);
	
	cute::HashDB db("files.csv");
	db.scanDirectory("test");
	db.writeCSV();

	return 0;
}





