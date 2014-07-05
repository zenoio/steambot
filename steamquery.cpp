#include "steamquery.h"

SteamQuery::SteamQuery(std::string configfile)
{
	std::string server, port;
	std::ifstream cfg(configfile);
	
	if (cfg.is_open()) {
		std::getline(cfg, server);
		std::getline(cfg, port);
		cfg.close();
		
		if (!server.empty() && !port.empty()) {
		
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
				if ((int)buffer[4] == 0x49)
					parseResponse(std::string(buffer, buffer + recvsize));
				else
					err = "error: malformed response";
			} else
				err = "error: server timeout";
		
			freeaddrinfo(res);
			close(socketfd);
		} else
			err = "error: invalid port or host specified";
	} else
		err = "error: couldn't open " + configfile;
}

void SteamQuery::parseResponse(std::string raw)
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
	response.id          = raw.substr(0, 2);
	response.cur         = raw[2];
	response.max         = raw[3];
	response.bots        = raw[4];
	response.servertype  = raw[5];
	response.enviroment  = raw[6];
	response.visibility  = raw[7];
	response.vac         = raw[8];
}
