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
#include <boost/lexical_cast.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>
#include <boost/system/error_code.hpp>

using boost::asio::ip::tcp;

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
  try
  {
    boost::array<char, 128> buf;
    buf.assign(' ');
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
    for(;;) {
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      boost::asio::write(socket,boost::asio::buffer("WELCOME\n"));
      while(true) {
        size_t len = socket.read_some(boost::asio::buffer(buf));
	std::cout.write(buf.data(), len);
      }

    }
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
