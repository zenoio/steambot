#include "ircbot.h"
#include <fstream>
#include <sstream>

int main(int argc, char *argv[])
{
	std::vector<std::string> cfg; // steamhost, steamport, nick, channel, server, port, owner

	if (argc != 2) {
		std::cout << "Usage: ./steambot [configfile]\n";
		return 1;
	} else {
		std::string filename = argv[1];
		std::ifstream cfgfile(filename);
		if (cfgfile.is_open()) {
			std::string temp;
			for (int i = 0; i < 9; i++)  {
				std::getline(cfgfile, temp);
				if (i > 1) // ignore first 2 lines
					cfg.push_back(temp.substr(0, temp.find(" ")));
			}
			
			IrcBot ircbot(cfg);
		} else {
			std::cout << "Usage: ./steambot [configfile]\n";
			return 1;
		}
	}

	return 0;
}
