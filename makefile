a.out: server.h server.cpp game.h game.cpp
	g++ server.cpp ai.cpp game.cpp -Lboost_asio -lboost_system -lpthread -g
