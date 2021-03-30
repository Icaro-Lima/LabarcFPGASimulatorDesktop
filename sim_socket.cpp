// Labarc FPGA Simulator Socket Server
// This file is derived from:
// verilator-3.922/examples/hello_world_c/sim_main.cpp
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2021

#include <iostream>
#include <iomanip>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace boost::asio;
using ip::tcp;
using boost::system::error_code;
using ip::host_name;
using std::string;
using std::istream;
using std::ostream;
using std::cout;
using std::cerr;
using std::setfill;
using std::setw;
using std::hex;
using std::endl;
using std::stoi;
using std::to_string;

extern void vinit(int argc, char** argv); // initializer Verilator
extern void vtick(); // Verilator timer action
extern void vcmd(unsigned short cmd, ostream& sout); // perform command and get response
extern void vdelete(); // Verilator destructor

// service for timer and socket
io_service io;

boost::posix_time::seconds interval(1);  // 1 second
deadline_timer timer(io, interval); // timer for the clock signal

void tick(const error_code& ) {
    vtick();
    // Reschedule the timer for 1 second in the future:
    timer.expires_at(timer.expires_at() + interval);
    // Posts the timer event
    timer.async_wait(tick);
}

// https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

//socket creation 
tcp::socket sock(io);
// buffer for string received by socket
streambuf binp;

void write_handle(const error_code&, size_t); // prototype to be used in read_handle

void read_handle(const error_code& err, size_t bytes_transferred)  {
    if (!err) {
         // get command from input stream
         istream is(&binp);
         char cmd_str[bytes_transferred]; // be sure we allocate enough memory
         is >> cmd_str;
         // read end-of-line character to clean up for next read
         char eol;
         is >> eol;
	 unsigned short cmd = stoi(cmd_str, 0, 2); // convert binary command string
         // prepare output string
         streambuf bout;
         ostream sout(&bout);
         sout << setfill('0') << hex;
	 // decode command
         vcmd(cmd,sout);
	 sout << '\r' << endl;  // needed for compatibility with JTAG server
         //write operation
         async_write(sock, bout, write_handle);
    } else {
         if (err != error::eof) {
             cerr << "read error: " << err.message() << endl;
         }
         // EOF, socket connection was terminated

         sock.close();

         // Destroy Verilog model
         vdelete();

         cerr << "___________________pronto____________________" << endl;
         // Fin
         exit(0);
    }
}

void write_handle(const error_code& err, size_t bytes_transferred) {
    if (!err) {
       // next read operation
       async_read_until(sock, binp, '\n', read_handle);
    } else {
       cerr << "write error: " << err.message() << endl;
       sock.close();
    }
}

void accept_handler(const error_code& error) {
  if (!error)
  {
    cout << "Connection accepted" << endl;
    // read operation
    async_read_until(sock, binp, '\n', read_handle);
  }
}

int main(int argc, char** argv, char** env) {

    vinit(argc, argv);

    //listener for new connection, let OS choose port number
    tcp::acceptor acceptor_(io, tcp::endpoint(tcp::v4(), 0));
    //waiting for connection
    acceptor_.async_accept(sock, accept_handler);
    int port = acceptor_.local_endpoint().port();

    // Schedule the timer for the first time:
    timer.async_wait(tick);

#ifdef LAD
    cout << "<h4>Agora digite: ./remote "
         << host_name() << " " << fpga << " </h4>" << endl;
#else 
    if (fork()==0) execl("./remote.bin", "remote.bin", to_string(port).c_str(), NULL);
#endif

    // Enter timer IO loop and never return.
    // The timer will fire for the first time 1 second from now.
    io.run();
}
