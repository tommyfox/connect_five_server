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
#include "server.h"

using boost::asio::ip::tcp;

bool is_hex(const char& c);

int convert_hex_to_int(char& c);
char convert_int_to_hex(int i);

Server::Server(int p) : port(p)
{  
	server_acceptor = new tcp::acceptor(server_io_service, tcp::endpoint(tcp::v4(), port)); 
    server_socket = new tcp::socket(server_io_service);
    server_game = new FIAR::Game(FIAR::WHITE, FIAR::RAND);
}

void Server::server_loop() {
	try
	{
		srand(time(NULL));
		boost::array<char, 400> buf;
		for(;;) {
			reset_connection();
			write_to_socket("WELCOME\n");
			display = false;
			bool is_connected = true;
			GameState game_state = INPROGRESS;
			std::string client_message;
			while(is_connected) {
				// reads data from the socket
				size_t len;
				try {
					len = server_socket->read_some(boost::asio::buffer(buf));
				}
				catch (...)  {
					len = 0;
					is_connected=false;
				}
				// stores the message in client_message
				client_message = "";
				for(int i = 0; i<len; i++) {
					if(buf[i]!=' '&&buf[i]!='\n'&&buf[i]!='\r') client_message += buf[i];
				}
				std::cout << client_message << std::endl;
				std::stringstream ai_move;
				if(client_message.size()!=0&&game_state==INPROGRESS) {
					// transforms client_message to lowercase
					for(int i = 0; i<client_message.length(); i++) {
						if(client_message[i]>='A'&&client_message[i]<='Z') client_message[i] += 32;
					}
					if(client_message=="exit") {
						is_connected=false;
						display = false;
						write_to_socket("\rOK\r\n");
					}
					else if(client_message=="display") {
						toggle_display();
						write_to_socket("\rOK\r\n");
					}
					else if(client_message=="human-ai") {
						write_to_socket("\rOK\r\n");
					}
					else if(client_message.substr(0,5)=="ai-ai") {
						write_to_socket("\rOK\r\n");
					}
					else if(client_message=="easy") {
						set_difficulty(EASY);
						write_to_socket("\rOK\r\n");
					}
					else if(client_message=="medium") {
						set_difficulty(MEDIUM);
						write_to_socket("\rOK\r\n");
					}
					else if(client_message=="hard") {
						set_difficulty(HARD);
						write_to_socket("\rOK\r\n");
					}
					else if(client_message=="undo") {
						server_game->undo();
						write_to_socket("\rOK\r\n");
					}
					else if(client_message[0]==';') {
						write_to_socket("\rOK\r\n");
					}
					else if(client_message.size()==2&&is_hex(client_message[0])&&is_hex(client_message[1])) {
						int row, column;
						row = convert_hex_to_int(client_message[0])-1;
						column = convert_hex_to_int(client_message[1])-1;
						if(!server_game->exec(row, column, FIAR::BLACK)) {
							write_to_socket("\rInvalid move\r\n");
						}
						else {
							write_to_socket("\rOK\r\n");
							int player_connected = server_game->calcStatus(row,column);
							std::cout << player_connected << std::endl;
							bool ai_valid_move = false;
							if(player_connected>=5) {
								display=false;
								game_state=CLIENTWIN;
							}
							else while(!ai_valid_move) {
								int row2 = rand() % 14;
								int column2 = rand() % 14;
								if(server_game->exec(row2, column2, FIAR::WHITE)) {
									ai_move << "\r" << convert_int_to_hex(row2+1) << convert_int_to_hex(column2+1) << "\r\n";
									ai_valid_move = true;
									int AI_connected = server_game->calcStatus(row2,column2);
									if(AI_connected>=5) {
										display=false;
										game_state=SERVERWIN;
									}
								}
								else {
									write_to_socket("\rOur AI messed up, have a free move!\r\n");
								}
								if(server_game->getNumberOfMoves() >= 225) {
									game_state=TIE;
									display=false;
								}
							}
						}
					}
					else if(client_message=="help") {
						help();
					}
					else {
						write_to_socket("\rInvalid command\r\n");
					}
				}
				// if the game_status flag is not INPROGRESS
				else if (game_state!=INPROGRESS) {
					if(client_message=="y") {
						write_to_socket("\rOK\r\n");
						server_game = new FIAR::Game(FIAR::WHITE, FIAR::RAND);
						game_state = INPROGRESS;
					}
					else if(client_message=="n") {
						write_to_socket("\rOK, disconnecting\r\n");
						is_connected = false;
					}
					else {
						write_to_socket("\rInvalid command\r\n");
					}
				}
				if(game_state==CLIENTWIN) {
					ai_move.flush();
					ai_move << "\rYou've won!\r\nWould you like to play again? (y/n)\r\n";
				}
				else if(game_state==SERVERWIN) {
					ai_move.flush();
					ai_move << "\rYou've lost :(\r\nWould you like to play again? (y/n)\r\n";
				}
				else if(game_state==TIE) {
					ai_move.flush();
					ai_move << "\rIt's a tie!\r\nWould you like to play again? (y/n)\r\n";
				}
				display_board(ai_move.str());
				ai_move.flush();
			}
		}
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Server::display_board(std::string ai_move) {
	if(display) {
		std::stringstream board;
		board << *server_game << ai_move;
		write_to_socket(board.str());
	}
	else write_to_socket(ai_move);
}

void Server::toggle_display() {
	if(display) display = false;
	else display = true;;
}

void Server::reset_connection() {
	if(server_acceptor->is_open()) {
      delete server_acceptor;
      server_acceptor = new tcp::acceptor(server_io_service, tcp::endpoint(tcp::v4(), port));
    }
    if(server_socket->is_open()) {
      delete server_socket;
      server_socket = new tcp::socket(server_io_service);
    }
    server_acceptor->accept(*server_socket);
    server_game = new FIAR::Game(FIAR::WHITE, FIAR::RAND);
}

void Server::write_to_socket() {
    boost::asio::write(*server_socket, boost::asio::buffer(message));
}

void Server::help() {
	//boost::asio::write(socket, boost::asio::buffer(" "));
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
