#include <stdio.h>
#include <vector>
#include <string.h>
#include <sys/time.h>
#include <curl/curl.h>

namespace cute
{

	class MultiCurl 
	{
		private:
			std::vector<CURL*> handles;
			CURLM *multi_handle;

			int still_running;
			int i;

			CURLMsg *msg;
			int mgs_left;

		public:
			int newHandle();
 
				






	}



};
