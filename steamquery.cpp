#include "steamquery.h"

SteamQuery::SteamQuery(std::string server, std::string port)
{
	struct addrinfo info, *res;
	struct timeval tv;
	memset(&info, 0, sizeof(info));
	unsigned char buffer[1024];
	int socketfd, recvsize;

	info.ai_family   = AF_UNSPEC  ;   // IPV4/IPV6
	info.ai_socktype = SOCK_DGRAM ;   // UDP
	
	tv.tv_sec        = 1          ;   // 1 second timeout
	tv.tv_usec       = 0          ;
	
	getaddrinfo(server.c_str(), port.c_str(), &info, &res);
	socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect(socketfd, res->ai_addr, res->ai_addrlen);
	
	unsigned char request[] = {0xff, 0xff, 0xff, 0xff, 0x54, 0x53, 0x6f, 0x75, 0x72, 0x63, 0x65, 0x20, 0x45, 0x6e, 0x67, 0x69, 0x6e, 0x65, 0x20, 0x51, 0x75, 0x65, 0x72, 0x79, 0x00};
	
	send(socketfd, request, sizeof(request), 0);
	setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	recvsize = recv(socketfd, buffer, 1024, 0);
	
	if (recvsize != EWOULDBLOCK && recvsize != EAGAIN && recvsize != 0) {
		raw = std::string(buffer, buffer + recvsize);
	
		int found = raw.find(std::string("\x00", 1));
		for (int i = 0; i < 4; i++) {
			raw = raw.substr(found + 1, std::string::npos);
			found = raw.find(std::string("\x00", 1));
		}
		raw = raw.substr(2, 4);
	
		int cur = raw[0];
		int max = raw[1];
	
		rdy = "Players: " + std::to_string(cur) + " / " + std::to_string(max);
	} else {
		rdy = "error: server timeout or malformed response";
	}
	
	freeaddrinfo(res);
	close(socketfd);
}
