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
  Server(int p);

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

  void reset_connection();

  void write_to_socket(std::string message);

  void set_difficulty(Difficulty){}
  void toggle_display();
  void help();
};

