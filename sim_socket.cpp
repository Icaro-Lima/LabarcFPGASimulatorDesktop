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
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace boost::asio;
using ip::tcp;
using boost::system::error_code;
using boost::posix_time::milliseconds;
using ip::host_name;
using std::string;
using std::istream;
using std::ostream;
using std::cout;
using std::cerr;
using std::setfill;
using std::setw;
using std::hex;
using std::setprecision;
using std::endl;
using std::stoi;
using std::to_string;
using std::thread;

extern int vinit(int argc, char** argv); // initialize Verilator, returns clock period
extern void vtick(); // Verilator timer action
extern void vcmd(unsigned short cmd, ostream& sout); // perform command and get response
extern void vdelete(); // Verilator destructor

// service for timer and socket
io_service io;

milliseconds *interval_ptr;
deadline_timer *timer_ptr; // timer for the clock signal

void tick(const error_code& ) {
    vtick();
    // Reschedule the timer for 1 second in the future:
    timer_ptr->expires_at(timer_ptr->expires_at() + *interval_ptr);
    // Posts the timer event
    timer_ptr->async_wait(tick);
}

// https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

//socket creation 
tcp::socket sock(io);
// buffer for string received by socket
streambuf binp;
// acceptor for socket opening
tcp::acceptor *acceptor_ptr;

// handle prototypes
void accept_handler(const error_code& error);
void read_handler(const error_code&, size_t);
void write_handler(const error_code&, size_t);

void exit_all() {
         sock.close();
	 timer_ptr->cancel();

         // Destroy Verilog model
         vdelete();

         cerr << "___________________pronto____________________" << endl;
         // Fin
         exit(0);
}

void accept_handler(const error_code& err) {
    if (!err)  {
       // read operation
       async_read_until(sock, binp, '\n', read_handler);
    } else {
       cerr << "accept error: " << err.message() << endl;
       exit_all();
    }
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
         cerr << "read error: " << err.message() << endl;
         exit_all();
    }
}

void write_handler(const error_code& err, size_t bytes_transferred) {
    if (!err) {
       sock.close();
       acceptor_ptr->async_accept(sock, accept_handler);  // accept next connection
    } else {
       cerr << "write error: " << err.message() << endl;
       exit_all();
    }
}

int port;

void independentThread() { 
    string cmd = 
#ifndef MINGW
                 "./"
#endif
                    "remote.bin " + to_string(port);
    std:system(cmd.c_str());
    exit(0);
}

int main(int argc, char** argv, char** env) {

    unsigned int divide_by, interval;

    if (argc>1) { 
       divide_by = atoi(argv[1]);
       // consume argv[1];
       argv[1] = argv[0];
       argc--;
       argv++;
    }
    else divide_by = 100000000;
    interval = divide_by / 100000; // timer interval in seconds
    float f = 500. / interval; // frequency in Hz

    if (divide_by > 500000000) {
       cerr << "Error: Clock frequency " << f << " Hz is below the allowed 0.1 Hz" << endl;
       exit(1);
    } else if (divide_by >= 500000 ) {
         cerr << setprecision(3) << f << " Hz" << endl;
    }  else {
       cerr << "Error: Clock frequency " << f << " Hz is higher than the allowed 100 Hz" << endl;
       exit(1);
    }        

    interval_ptr = new milliseconds(interval);
    timer_ptr = new deadline_timer(io, *interval_ptr);

    vinit(argc, argv);

    //listener for new connection, let OS choose port number
    acceptor_ptr = new tcp::acceptor(io, tcp::endpoint(tcp::v4(), 0));
    port = acceptor_ptr->local_endpoint().port();

    //waiting for first connection
    acceptor_ptr->async_accept(sock, accept_handler);

    // Schedule the timer for the first time:
    timer_ptr->async_wait(tick);

#ifdef LAD
    string name = host_name();
    cerr << "<h4>Agora digite: ./remote "
         << name.substr(0,name.find('.')) << " " << port << " </h4>" << endl;
    // need to use cerr because only stderr is directed to log file
#else
    thread t(independentThread);
    t.detach();
#endif

    // Enter timer IO loop and never return.
    // The timer will fire for the first time 1 second from now.
    io.run();
#ifndef LAD
    t.join();
#endif
}
