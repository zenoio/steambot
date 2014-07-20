all:
	g++ -std=c++11 main.cpp ircbot.cpp steamquery.cpp httprequest.cpp -o steambot
debug:
	g++ -g -std=c++11 main.cpp ircbot.cpp steamquery.cpp httprequest.cpp -o steambot_dbg
clean:
	rm -f steambot*
