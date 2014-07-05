#include "ircbot.h"
#include "steamquery.h"

IrcBot::IrcBot(std::vector<std::string> cfg_)
{
	cfg = cfg_;
	joined = false;
	connectionClosed = false;
	int status, recvsize;
	
	struct addrinfo info, *res;
	memset(&info, 0, sizeof(info));
	char buffer[1024];
	
	info.ai_family   = AF_UNSPEC;   // IPV4/IPV6
	info.ai_socktype = SOCK_STREAM; // TCP
	
	status = getaddrinfo(cfg[4].c_str(), cfg[5].c_str(), &info, &res);
	if (status != 0)
		connectionClosed = true;
	socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	status = connect(socketfd, res->ai_addr, res->ai_addrlen);
	if (status != 0)
		connectionClosed = true;
	
	sendRaw("USER " + cfg[2] + " 0 * :" + cfg[2]);
	sendRaw("NICK " + cfg[2]);
	
	while (!connectionClosed) {
		recvsize = recv(socketfd, buffer, 1024, 0);
		recievedMessage(std::string(buffer, buffer + recvsize));
		memset(&buffer, 0, sizeof(buffer));
	}
	
	freeaddrinfo(res);
	close(socketfd);
}

void IrcBot::sendRaw(std::string msg)
{
	msg = msg + "\r\n";
	std::cout << ">> " << msg;
	send(socketfd, msg.c_str(), strlen(msg.c_str()), 0);
}

void IrcBot::sendMsg(std::string msg, std::string chan)
{
	msg = "PRIVMSG " + chan + " :" + msg;
	sendRaw(msg);
}
void IrcBot::recievedMessage(std::string msg)
{
	std::vector<std::string> lines;
	std::string separator = "\r\n";
	
	int found = msg.find(separator);
	while (found != std::string::npos){
		if (found > 0){
			lines.push_back(msg.substr(0, found));
		}
		msg = msg.substr(found + 2);
		found = msg.find(separator);
    }
	if (msg.length() > 0){
		lines.push_back(msg);
	}

	for (int i = 0; i < lines.size(); i++) {
		std::cout << "<< " << lines[i] << "\n";
		if (lines[i].find("PING") == 0) {
			sendRaw("PONG :" + lines[i].substr(lines[i].find(":") + 1, std::string::npos));
		} else if (!joined && lines[i].substr(lines[i].find(" ") + 1, 3) == "001") {
			sendRaw("JOIN " + cfg[3]);
			joined = true;
		} else if (lines[i].find("PRIVMSG") == lines[i].find(" ") + 1) {
			std::string nick = lines[i].substr(1, lines[i].find("!") - 1);
			std::string msg_ = lines[i].substr(lines[i].find(":", 2) + 1, std::string::npos);
			std::string chan = lines[i].substr(lines[i].find("PRIVMSG ") + 8, (lines[i].find(":", 2) - 1) - (lines[i].find("PRIVMSG ") + 8));
			if (chan.find("#") == std::string::npos)
				chan = nick;
			messageHandler(nick, msg_, chan);
		}
	}
}

void IrcBot::messageHandler(std::string nick, std::string msg, std::string chan)
{
	std::transform(nick.begin(), nick.end(), nick.begin(), ::tolower);
	std::transform(msg.begin(), msg.end(), msg.begin(), ::tolower);
	std::transform(chan.begin(), chan.end(), chan.begin(), ::tolower);
	
	if (msg.find("!p") == 0 || msg.find("!players") == 0) {
		SteamQuery query(cfg[0], cfg[1]);
		if (query.err.empty()) {
			sendMsg("players: " + std::to_string((int)query.response.cur[0]) + " / " + std::to_string((int)query.response.max[0]) + " -- other info - name: " + query.response.name + " - os: " + query.response.enviroment, chan);
		} else {
			sendMsg(query.err, chan);
		}
	} else if (nick == cfg[6] && msg.find("!q") == 0) {
		sendRaw("QUIT :b-b-baka " + cfg[6] + "!!");
		connectionClosed = true;
	}
}
