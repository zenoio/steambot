#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class HttpRequest
{
public:
	std::string err;
	std::string title;

	HttpRequest(std::string server, std::string port, std::string index);
	void parseResponse(std::string str);
};

#endif //HTTPREQUEST_H