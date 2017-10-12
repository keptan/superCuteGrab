#include "multiCurl.h"

namespace cute
{
	int MultiCurl :: newHandle(std::string u)
	{
		vector.push_back(new CURL*curl_easy_init();)
		curl_easy_setopt(handles.push_back(),CURLOPT_URL,u.c_str());

	}
