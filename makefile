a.out: server.h server.cpp game.h game.cpp
	g++ server.cpp game.cpp -Lboost_asio -lboost_system -lpthread -g
