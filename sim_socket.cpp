// Labarc FPGA Simulator Socket Server
// This file is derived from:
// verilator-3.922/examples/hello_world_c/sim_main.cpp
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2021

#define BOOST_BIND_GLOBAL_PLACEHOLDERS
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
// acceptor for socket opening
tcp::acceptor *acceptor_ptr;

// prototypes to be used in read_handle
void accept_handler(const error_code& error);
void write_handler(const error_code&, size_t);

void exit_all() {
         sock.close();

         // Destroy Verilog model
         vdelete();

         cerr << "___________________pronto____________________" << endl;
         // Fin
         exit(0);
}

void read_handler(const error_code& err, size_t bytes_transferred)  {
    if (!err) {
         // get command from input stream
         istream is(&binp);
         char cmd_str[bytes_transferred], // be sure we allocate enough memory
	      ip_str[bytes_transferred];
         is >> cmd_str >> ip_str;
         // read end-of-line character to clean up for next read
         char eol;
         is >> eol;
	 if (cmd_str[0] == 'e') exit_all(); // if command is "exit"
	 unsigned short cmd = stoi(cmd_str, 0, 2); // convert binary command string
         // prepare output string
         streambuf bout;
         ostream sout(&bout);
         sout << setfill('0') << hex;
         vcmd(cmd,sout);  // pass command to Verilator and get response
	 sout << '\r' << endl;  // needed for compatibility with JTAG server
         //write operation
         async_write(sock, bout, write_handler);
    } else {
         if (err == error::eof) {  // running in localhost - exit server normally
            exit_all();
         } else if ( err == error::connection_reset) { // running in LAD
            sock.close();
            // accept new connection
            acceptor_ptr->async_accept(sock, accept_handler);
         } else {
             cerr << "read error: " << err.message() << endl;
             exit_all();
         }
    }
}

void write_handler(const error_code& err, size_t bytes_transferred) {
    if (!err) {
       // next read operation
       async_read_until(sock, binp, '\n', read_handler);
    } else {
       cerr << "write error: " << err.message() << endl;
       sock.close();
    }
}

void accept_handler(const error_code& error) {
  if (!error)
  {
    // read operation
    async_read_until(sock, binp, '\n', read_handler);
  }
}

int main(int argc, char** argv, char** env) {

    vinit(argc, argv);

    //listener for new connection, let OS choose port number
    acceptor_ptr = new tcp::acceptor(io, tcp::endpoint(tcp::v4(), 0));
    int port = acceptor_ptr->local_endpoint().port();

    //waiting for connection
    acceptor_ptr->async_accept(sock, accept_handler);

    // Schedule the timer for the first time:
    timer.async_wait(tick);

#ifdef LAD
    string name = host_name();
    cerr << "<h4>Agora digite: ./remote "
         << name.substr(0,name.find('.')) << " " << port << " </h4>" << endl;
    // need to use cerr because only stderr is directed to log file
#else 
    if (fork()==0) execl("./remote.bin", "remote.bin", to_string(port).c_str(), NULL);
#endif

    // Enter timer IO loop and never return.
    // The timer will fire for the first time 1 second from now.
    io.run();
}
