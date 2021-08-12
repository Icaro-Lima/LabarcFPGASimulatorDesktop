#include "communicator.h"
// This program is based in part on
// https://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/echo/blocking_tcp_echo_client.cpp

// For compatibility with web server socket client,
// the connection is opened and closed for each send_and_receive.
// However, in Windows opening a connection takes an excessive amount of time
// (7 seconds have been reported), so in Windows the connection is opened only once.

communicator::communicator(char *host, char *port) :
   sock(io_serve),
   resolver(io_serve) {
   try
   {
     host_port =  resolver.resolve({host, port});
#ifdef MINGW
     connect(sock, host_port);
#endif
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
#ifndef MINGW
    connect(sock, host_port);
#endif
    // send request string
    string req = r + "\n";
    const char *request = req.c_str();
    size_t request_length = std::strlen(request);
    boost::asio::write(sock, boost::asio::buffer(request, request_length));

    // reveive a string of characters  
    error_code error;
    streambuf response;

    boost::asio::read_until(sock, response, "\r\n", error);
    ss << &response;
    if (error != boost::system::errc::success && error != boost::asio::error::eof)
       throw boost::system::system_error(error);
  }
  catch (std::exception& e)
  {
    std::cerr << "send_and_rec exception: " << e.what() << "\n";
    exit(9); // exit in case of connection error
  }

  string s = ss.str();
  if (s.length() < max_length) strcpy(cs, s.c_str());
  else strcpy(cs, s.substr(s.length()-max_length+1).c_str()); // copy end of string
  return cs;
}

