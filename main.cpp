#include "ircbot.h"
#include <fstream>
#include <sstream>

int main(int argc, char *argv[])
{
	std::vector<std::string> cfg; // serverlist, nick, channel, server, port, owner

	if (argc != 2) {
		std::cout << "Usage: ./steambot [configfile]\n";
		return 1;
	} else {
		std::string filename = argv[1];
		std::ifstream cfgfile(filename);
		if (cfgfile.is_open()) {
			std::string temp;
			for (int i = 0; i < 8; i++)  {
				if (!std::getline(cfgfile, temp)) {
					std::cout << "There was something wrong with the configuration file...\n";
					return 1;
				} else if (i > 1) // ignore first 2 lines
					cfg.push_back(temp.substr(0, temp.find(" ")));
			}
			if (cfg.size() == 6)
				IrcBot ircbot(cfg);
		} else {
			std::cout << "Usage: ./steambot [configfile]\n";
			return 1;
		}
	}

	return 0;
}
