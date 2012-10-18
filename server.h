#include "game.h"

using boost::asio::ip::tcp;

enum Difficulty {
  EASY = 0,
  MEDIUM,
  HARD
};

enum GameState {
  INPROGRESS,
  SERVERWIN,
  CLIENTWIN,
  TIE
};

class Server {
public:
  Server(int p)
  : port(p)
  {  server_acceptor = new tcp::acceptor(server_io_service, tcp::endpoint(tcp::v4(), port)); 
     server_socket = new tcp::socket(server_io_service);
     server_game = new FIAR::Game(FIAR::WHITE, FIAR::RAND);
  }

  ~Server() { delete server_acceptor; delete server_socket; delete server_game; }

  void server_loop();

private:
  int 				port;
  bool				display;
  FIAR::Game*			server_game;

  boost::asio::io_service 	server_io_service;
  tcp::socket* 			server_socket;
  tcp::acceptor* 		server_acceptor;

  void display_board(std::string ai_move);

  void reset_connection() {
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

  void write_to_socket(std::string message) {
    boost::asio::write(*server_socket, boost::asio::buffer(message));
  }

  void set_difficulty(Difficulty){}
  void toggle_display();
  void help();
};

