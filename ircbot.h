#ifndef IRCBOT_H
#define IRCBOT_H

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class IrcBot
{
private:
	void sendRaw(std::string msg);
	void sendMsg(std::string, std::string chan);
	void recievedMessage(std::string msg);
	void messageHandler(std::string nick, std::string msg, std::string chan);
	
	int socketfd;
	bool joined, connectionClosed;
	std::string channel, steamhost, steamport;
public:
	IrcBot(std::string nick, std::string channel_, std::string server, std::string port, std::string steamhost, std::string steamport);
};

#endif // IRCBOT_H
