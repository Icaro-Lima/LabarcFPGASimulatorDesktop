#include "communicator.h"
// This program is based in part on
// https://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/echo/blocking_tcp_echo_client.cpp

communicator::communicator(char *host, char *port) :
   sock(io_serve),
   resolver(io_serve) {
   try
   {
     host_port =  resolver.resolve({host, port});
   }
   catch (exception& e)
   {
     cerr << "Exception: " << e.what() << "\n";
     exit(7); // exit in case of resolver error
   } 
}

char *communicator::send_and_rec(string r) {
  stringstream ss;
  try
  {
    connect(sock, host_port);
    // send request string
    string req = r + "\n";
    const char *request = req.c_str();
    size_t request_length = std::strlen(request);
    boost::asio::write(sock, boost::asio::buffer(request, request_length));

    // reveive a string of characters  
    error_code error;
    streambuf response;

    // Read until EOF, writing data to output as we go.
    while (boost::asio::read(sock, response,
           boost::asio::transfer_at_least(1), error)) {
      ss << &response;
    }
    if (error != boost::asio::error::eof)
       throw boost::system::system_error(error);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    exit(9); // exit in case of connection error
  }

  string s = ss.str();
  if (s.length() < max_length) strcpy(cs, s.c_str());
  else strcpy(cs, s.substr(s.length()-max_length+1).c_str()); // copy end of string
  return cs;
}

