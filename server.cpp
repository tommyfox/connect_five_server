/*************************************\
 *  Project: Project 2 - Five-in-a-Row 
 *
 *  Coder: Chris Polansky
 *  Contact: chris.polansky@tamu.edu
 *
 *  Editor: VIM - Vi IMproved 7.2
 *  Compiler: g++-4.6 4.6.3
 *
 *  License: Proprietary
 * 
\*************************************/

#include <cstdlib>
#include <iostream>
#include <cerrno>

#include <string>
#include <sstream>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>
#include "game.h"

using boost::asio::ip::tcp;

bool is_hex(const char& c);

enum Difficulty {
	EASY = 0,
	MEDIUM,
	HARD
};

enum Hex {
	ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
	EIGHT, NINE, A, B, C, D, E, F
};

class Server {
public:
	Server(int p) {port = p;}

	void server_loop();

private:
	int port;
	bool display;

	void set_difficulty(Difficulty){}
	void toggle_display();
	void undo(){}
	void move(Hex, Hex){}
	void comment(std::string){}
};

void Server::server_loop() {
try
	{
		boost::array<char, 400> buf;
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
		for(;;) {
      			tcp::socket socket(io_service);
      			acceptor.accept(socket);

      			FIAR::Game server_game;
      			boost::asio::write(socket,boost::asio::buffer("WELCOME\n"));
      			bool first_go = true;
      			display = false;
			bool is_connected = true;
			std::string client_message;
			while(is_connected) {
				// reads data from the socket
				size_t len = socket.read_some(boost::asio::buffer(buf));
				// stores the message in client_message
				client_message = "";
				for(int i = 0; i<len; i++) {
					if(buf[i]!=' '&&buf[i]!='\n'&&buf[i]!='\r') client_message += buf[i];
				}
				if(client_message.size()!=0) {
					// transforms client_message to lowercase
					std::transform(client_message.begin(), client_message.end(), client_message.begin(), ::tolower);
					//std::cout << "Transformed client message: " << client_message << std::endl;
					if(client_message=="exit") {
						is_connected=false;
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message=="display") {
						toggle_display();
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
        				}
					else if(client_message=="easy") {
						set_difficulty(EASY);
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message=="medium") {
						set_difficulty(MEDIUM);
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message=="hard") {
						set_difficulty(HARD);
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message=="undo") {
						undo();
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message.size()==2&&is_hex(client_message[0])&&is_hex(client_message[1])) {
						boost::asio::write(socket, boost::asio::buffer("\rOK, accepted hex move\r\n"));
					}
					else {
						boost::asio::write(socket, boost::asio::buffer("\rInvalid command\r\n"));
					}
				}
				if(display) {
					std::stringstream board;
					board.flush();
					board << server_game;
					std::cout << board.str();
					boost::asio::write(socket, boost::asio::buffer(board.str()));
				}
			}
		}

	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Server::toggle_display() {
	if(display) display = false;
	else display = true;;
}

int main(int argc, char* argv[]) {
  int port;
  if(argc != 2) {
    std::cout << "FIAR-server: " << strerror(EINVAL) << std::endl;
    std::cout << "usage: FIAR-server [port]\n";
    return EXIT_FAILURE;
  }
  else {
    try {
      port = boost::lexical_cast<int>(argv[1]);
    }
    catch(boost::bad_lexical_cast&) {
      std::cout << "FIAR-server: " << strerror(EINVAL) << std::endl;
      std::cout << "usage: FIAR-server [port]\n";
      return EXIT_FAILURE;
    }
  }
	Server serv(port);
	serv.server_loop();

  return EXIT_SUCCESS;
}

bool is_hex(const char& c) {
	if(c>='a'&&c<='f'||c>='0'&&c<='f') return true;
	return false;
}
