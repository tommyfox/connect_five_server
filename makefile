a.out: server.h server.cpp game.h game.cpp ai.h ai.cpp aiminmax.cpp
	g++ server.cpp ai.cpp aiminmax.cpp game.cpp -Lboost_asio -lboost_system -lpthread -g
