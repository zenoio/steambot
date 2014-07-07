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
		
	tv.tv_sec        = 2          ;   // 2 second timeout
	tv.tv_usec       = 0          ;
		
	getaddrinfo(server.c_str(), port.c_str(), &info, &res);
	socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect(socketfd, res->ai_addr, res->ai_addrlen);
	
	unsigned char request[] = {0xff, 0xff, 0xff, 0xff, 0x54, 0x53, 0x6f, 0x75, 0x72, 0x63, 0x65, 0x20, 0x45, 0x6e, 0x67, 0x69, 0x6e, 0x65, 0x20, 0x51, 0x75, 0x65, 0x72, 0x79, 0x00};
	
	send(socketfd, request, sizeof(request), 0);
	setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	recvsize = recv(socketfd, buffer, 1024, 0);
	
	if (recvsize != EWOULDBLOCK && recvsize != EAGAIN && recvsize > 15) {
		if (buffer[4] == 0x49)
			parseResponse(std::string(buffer, buffer + recvsize), port);
		else
			err = "error: malformed response";
	} else
		err = "error: server timeout";
		
	freeaddrinfo(res);
	close(socketfd);
}

void SteamQuery::parseResponse(std::string raw, std::string &queryport)
{	
	response.header      = raw[4];
	response.protocol    = raw[5];
	raw = raw.substr(6, std::string::npos);
	response.name        = raw.substr(0, raw.find(std::string("\x00", 1)));
	raw = raw.substr(raw.find(std::string("\x00", 1)) + 1, std::string::npos);
	response.map         = raw.substr(0, raw.find(std::string("\x00", 1)));
	raw = raw.substr(raw.find(std::string("\x00", 1)) + 1, std::string::npos);
	response.folder      = raw.substr(0, raw.find(std::string("\x00", 1)));
	raw = raw.substr(raw.find(std::string("\x00", 1)) + 1, std::string::npos);
	response.game        = raw.substr(0, raw.find(std::string("\x00", 1)));
	raw = raw.substr(raw.find(std::string("\x00", 1)) + 1, std::string::npos);
	response.id          = /*raw.substr(0, 2)*/ 0;
	response.cur         = raw[2];
	response.max         = raw[3];
	response.bots        = raw[4];
	response.servertype  = raw[5];
	response.enviroment  = raw[6];
	response.visibility  = raw[7];
	response.vac         = raw[8];
	raw = raw.substr(9, std::string::npos);
	response.version     = raw.substr(0, raw.find(std::string("\x00", 1)));
	raw = raw.substr(raw.find(std::string("\x00", 1)) + 1, std::string::npos);
	if (!raw.empty()) {
		response.edf = raw[0];
		raw = raw.substr(1, std::string::npos);
		if (response.edf & 0x80) {
			response.edf_data.port = ((unsigned char)raw[1] << 8) | (unsigned char)raw[0];
			raw = raw.substr(2, std::string::npos);
		} else
			response.edf_data.port = (short)std::stol(queryport);
		if (response.edf & 0x10) {
			response.edf_data.steamid = /*raw.substr(0, 8)*/ 0;
			raw = raw.substr(8, std::string::npos);
		}
		if (response.edf & 0x40) {
			response.edf_data.sourcetv_port = ((unsigned char)raw[1] << 8) | (unsigned char)raw[0];
			raw = raw.substr(2, std::string::npos);
			response.edf_data.sourcetv_name = raw.substr(0, raw.find(std::string("\x00", 1)));
			raw = raw.substr(raw.find(std::string("\x00", 1)) + 1, std::string::npos);
		}
		if (response.edf & 0x20) {
			response.edf_data.keywords = raw.substr(0, raw.find(std::string("\x00", 1)));
			raw = raw.substr(raw.find(std::string("\x00", 1)) + 1, std::string::npos);
		}
		if (response.edf & 0x01) {
			response.edf_data.gameid = /*raw.substr(0, 8)*/ 0;
		}
	} else
		response.edf_data.port = (short)std::stol(queryport);
}
