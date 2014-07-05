#ifndef STEAMQUERY_H
#define STEAMQUERY_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

class SteamQuery
{
public:
	struct steam_response { // https://developer.valvesoftware.com/wiki/Server_queries
		std::string header;
		std::string protocol;
		std::string name;
		std::string map;
		std::string folder;
		std::string game;
		std::string id;
		std::string cur;
		std::string max;
		std::string bots;
		std::string servertype;
		std::string enviroment;
		std::string visibility;
		std::string vac;
	};
	
	SteamQuery(std::string configfile);
	void parseResponse(std::string raw);
	std::string err;
	steam_response response;
};

#endif //STEAMQUERY_H