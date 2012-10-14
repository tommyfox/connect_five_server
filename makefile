a.out: server.cpp game.h game.cpp
	g++ server.cpp game.cpp -Lboost_asio -lboost_system -lpthread
