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
		unsigned char header;
		unsigned char protocol;
		std::string name;
		std::string map;
		std::string folder;
		std::string game;
		short id;
		unsigned char cur;
		unsigned char max;
		unsigned char bots;
		unsigned char servertype;
		unsigned char enviroment;
		unsigned char visibility;
		unsigned char vac;
		std::string version;
		unsigned char edf;
		struct edf_data {
			short port;
			long long steamid;
			short sourcetv_port;
			std::string sourcetv_name;
			std::string keywords;
			long long gameid;
		} edf_data;
	};
	
	struct arma_keywords {
		std::string battleeye;
		std::string requiredversion;
		std::string requiredbuildno;
		std::string serverstate;
		std::string difficulty;
		std::string equalmodrequired;
		std::string lock;
		std::string verifysignatures;
		std::string dedicated;
		std::string gametype;
		std::string language;
		std::string longlat;
		std::string platform;
	};
	
	SteamQuery(std::string server, std::string port);
	void parseResponse(std::string raw, std::string &queryport);
	std::string err;
	steam_response response;
};

#endif //STEAMQUERY_H