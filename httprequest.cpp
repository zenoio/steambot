#include "httprequest.h"

HttpRequest::HttpRequest(std::string server, std::string port, std::string index)
{
	int socketfd, recvsize;
	struct addrinfo info, *res;
	struct timeval tv;
	memset(&info, 0, sizeof(info));
	char buffer[1024];
	std::string str;
	
	info.ai_family   = AF_UNSPEC   ;   // IPV4/IPV6
	info.ai_socktype = SOCK_STREAM ;   // TCP
	
	tv.tv_sec        = 1           ;   // 1 second timeout
	tv.tv_usec       = 0           ;
	
	getaddrinfo(server.c_str(), port.c_str(), &info, &res);
	socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect(socketfd, res->ai_addr, res->ai_addrlen);
	
	std::string msg = "GET /" + index + " HTTP/1.1\nHost: " + server + "\n\n";
	
	send(socketfd, msg.c_str(), strlen(msg.c_str()), 0);
	setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	recvsize = recv(socketfd, buffer, 1024, 0);
	int i = 0;
	
	while (recvsize != EWOULDBLOCK && recvsize != EAGAIN && recvsize > 0) {
		str += std::string(buffer, buffer + recvsize);
		memset(&buffer, 0, sizeof(buffer));
		recvsize = recv(socketfd, buffer, 1024, 0);
		i++;
	}
	
	if (i = 0)
		err = "error: server timeout";
	else
		parseResponse(str);
	
	freeaddrinfo(res);
	close(socketfd);
}

void HttpRequest::parseResponse(std::string str)
{
	std::string strLower = str;
	std::transform(strLower.begin(), strLower.end(), strLower.begin(), ::tolower);
	int titlestart = strLower.find("<title>") + 7;
	int titleend = strLower.find("</title>");
	std::cout << titlestart << titleend << "\n";
	if (titlestart != std::string::npos && titleend != std::string::npos) {
		title = str.substr(titlestart, titleend - titlestart);
	} else
		err = "error: no title found";
}