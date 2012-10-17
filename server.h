using boost::asio::ip::tcp;

enum Difficulty {
  EASY = 0,
  MEDIUM,
  HARD
};

class Server {
public:
  Server(int p)
  : port(p) /*server_acceptor(server_io_service, tcp::endpoint(tcp::v4(), port)),*/
    /*server_socket(server_io_service)*/ {server_acceptor = new tcp::acceptor(server_io_service, tcp::endpoint(tcp::v4(), port)); 
  server_socket = new tcp::socket(server_io_service);
  }

  void server_loop();

private:
  int 				port;
  bool				display;

  boost::asio::io_service 	server_io_service;
  tcp::socket* 			server_socket;
  tcp::acceptor* 		server_acceptor;

  void reset_connection() {
    if(server_acceptor->is_open()) {
      std::cerr << "server acceptor is open\n";
      delete server_acceptor;
      std::cerr << "server acceptor is closed\n";
      server_acceptor = new tcp::acceptor(server_io_service, tcp::endpoint(tcp::v4(), port));
    }
    if(server_socket->is_open()) {
std::cerr << "server socket is open\n";
      delete server_socket;
      server_socket = new tcp::socket(server_io_service);
    }
    //server_socket.open(tcp::v4());
    server_acceptor->accept(*server_socket);
  }

  void write_to_socket(std::string message) {
    boost::asio::write(*server_socket, boost::asio::buffer(message));
  }

  void set_difficulty(Difficulty){}
  void toggle_display();
  void help();
};

