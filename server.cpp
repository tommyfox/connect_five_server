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

#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>

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
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
    for(;;) {
      tcp::socket socket(io_service);
      acceptor.accept(socket);

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
