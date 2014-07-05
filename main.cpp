#include "ircbot.h"

int main()
{
	std::string nick       = "steambot"         ;
	std::string channel    = "#papuwarfare"     ;
	std::string server     = "irc.quakenet.org" ;
	std::string port       = "6667"             ;
	
	std::string owner      = "viderizer"        ;

	IrcBot ircbot(nick, channel, server, port, owner);

	return 0;
}
