#ifndef STEAMQUERY_H
#define STEAMQUERY_H

#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class SteamQuery
{
public:
	SteamQuery(std::string server, std::string port);
	std::string raw, rdy;
};

#endif //STEAMQUERY_H