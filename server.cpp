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
#include <stdlib.h>
#include <iostream>
#include <cerrno>

#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <boost/lexical_cast.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>
#include "game.h"

using boost::asio::ip::tcp;

bool is_hex(const char& c);

int convert_hex_to_int(char& c);
char convert_int_to_hex(int i);

enum Difficulty {
	EASY = 0,
	MEDIUM,
	HARD
};

class Server {
public:
	Server(int p) {port = p;}

	void server_loop() throw(std::out_of_range);

private:
	int port;
	bool display;

	void set_difficulty(Difficulty){}
	void toggle_display();
	void comment(std::string){}
};

void Server::server_loop() throw(std::out_of_range) {
	try
	{
		srand(time(NULL));
		boost::array<char, 400> buf;
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
		for(;;) {
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			FIAR::Game server_game;
			boost::asio::write(socket,boost::asio::buffer("WELCOME\n"));
			display = false;
			bool is_connected = true;
			bool first_time = true;
			std::string client_message;
			while(is_connected) {
				// reads data from the socket
				size_t len;
				try {
					len = socket.read_some(boost::asio::buffer(buf));
				}
				catch (...)  {
					len = 0;
					is_connected=false;
				}
				// stores the message in client_message
				std::stringstream ai_move;
				client_message = "";
				for(int i = 0; i<len; i++) {
					if(buf[i]!=' '&&buf[i]!='\n'&&buf[i]!='\r') client_message += buf[i];
				}
				std::cout << client_message << std::endl;
				if(!first_time&&client_message.size()!=0) {
					// transforms client_message to lowercase
					for(int i = 0; i<client_message.length(); i++) {
						if(client_message[i]>='A'&&client_message[i]<='Z') client_message[i] += 32;
					}
					if(client_message=="exit") {
						is_connected=false;
						display = false;
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message=="display") {
						toggle_display();
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message=="human-ai") {
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message.substr(0,5)=="ai-ai") {
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
						server_game.undo();
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message[0]==';') {
						boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
					}
					else if(client_message.size()==2&&is_hex(client_message[0])&&is_hex(client_message[1])) {
						int row, column;
						column = convert_hex_to_int(client_message[0])-1;
						row = convert_hex_to_int(client_message[1])-1;
						if(!server_game.exec(row, column, FIAR::BLACK)) {
							boost::asio::write(socket, boost::asio::buffer("\rInvalid move\r\n"));
						}
						else {
							boost::asio::write(socket, boost::asio::buffer("\rOK\r\n"));
							int player_connected = server_game.calcStatus(row,column,FIAR::ALL);
							std::cout << player_connected << std::endl;
							bool ai_valid_move = false;
							if(player_connected>=5) {
								std::stringstream board;
								board << server_game << "\rYou've won!\r\n";
								if(display) boost::asio::write(socket, boost::asio::buffer(board.str()));
								display=false;
							}
							else while(!ai_valid_move) {
								int row2 = rand() % 14;
								int column2 = rand() % 14;
								if(server_game.exec(row2, column2, FIAR::WHITE)) {
									ai_move << "\r" << convert_int_to_hex(column2+1) << convert_int_to_hex(row2+1) << "\r\n";
									ai_valid_move = true;
									int AI_connected = server_game.calcStatus(row2,column2,FIAR::ALL);
									if(AI_connected>=5) {
										std::stringstream board;
										board << server_game << "\rYou've lost :(\r\n";
										boost::asio::write(socket, boost::asio::buffer(board.str()));
										display=false;
									}
								}
								else {
									boost::asio::write(socket, boost::asio::buffer("\rOur AI messed up, have a free move!\r\n"));
								}
							}
						}
					}
					else {
						boost::asio::write(socket, boost::asio::buffer("\rInvalid command\r\n"));
					}
				}
				else {
					first_time = false;
					boost::asio::write(socket, boost::asio::buffer("\r"));
				}
				if(display) {
					std::stringstream board;
					board.flush();
					board << server_game << ai_move.str();
					//std::cout << board.str();
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
	if(c>='a'&&c<='f'||c>='0'&&c<='9') return true;
	return false;
}

int convert_hex_to_int(char& c) {
	if(c>='0'&&c<='9') return c-48;
	else if(c=='a') return 10;
	else if(c=='b') return 11;
	else if(c=='c') return 12;
	else if(c=='d') return 13;
	else if(c=='e') return 14;
	else return 15;
}

char convert_int_to_hex(int i) {
	if(i>=0&&i<=9) return i+48;
	else if(i==10) return 'a';
	else if(i==11) return 'b';
	else if(i==12) return 'c';
	else if(i==13) return 'd';
	else if(i==14) return 'e';
	else return 'f';
}
