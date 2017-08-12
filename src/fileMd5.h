#include <stdio.h>
#include <openssl/md5.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

std::string hashFile(const char* path)
{

	FILE *fileIn = fopen(path,"r");
	unsigned char hash[MD5_DIGEST_LENGTH];
	int i;
	MD5_CTX mdContext;
	int bytes;
	unsigned char data[1024];
	char temp[2];

	 char hashOut[100];

	if (fileIn == NULL) {
		printf (" can't be opened\n");
		return 0;
		
	}

	MD5_Init (&mdContext);
	while ((bytes = fread (data, 1,1 , fileIn)) != 0)
		MD5_Update (&mdContext, data, bytes);
	MD5_Final (hash,&mdContext);

	
	std::string s;
	
	for (i = 0; i < MD5_DIGEST_LENGTH; i++) { 
		sprintf(&hashOut[i*2], "%02x",(unsigned int)hash[i]); 
		

	}
	
	s = hashOut;
	
	return s;

}
	




	
