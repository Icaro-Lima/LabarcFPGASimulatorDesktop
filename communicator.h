#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using std::string;
using std::stringstream;
using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using boost::asio::ip::tcp;
using boost::asio::io_service;
using boost::asio::connect;
using boost::asio::streambuf;
using boost::system::error_code;

class communicator {
  io_service io_serve;
  tcp::socket sock;
  tcp::resolver resolver;
  boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator host_port;
  enum { max_length = 2048 };
public:
  communicator(char *host, char *port);
  const char *send_and_rec(string r); // send a request string and receive the reply
};

