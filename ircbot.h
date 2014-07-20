#ifndef IRCBOT_H
#define IRCBOT_H

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <regex>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class IrcBot
{
private:
	struct server_info {
		std::string prefix;
		std::string server;
		std::string port;
		inline server_info(std::string p, std::string s, std::string po) {
			prefix = p;
			server = s;
			port = po;
		}
	};

	void sendRaw(std::string msg);
	void sendMsg(std::string, std::string chan);
	void recievedMessage(std::string msg);
	void messageHandler(std::string nick, std::string msg, std::string chan);
	
	int socketfd;
	bool joined, connectionClosed;
	std::vector<std::string> cfg;
public:
	IrcBot(std::vector<std::string> cfg_);
};

#endif // IRCBOT_H
