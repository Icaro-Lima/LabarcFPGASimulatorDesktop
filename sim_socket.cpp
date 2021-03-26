#include <iostream>
#include <iomanip>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::setfill;
using std::setw;
using std::hex;
using std::endl;

// Include common Verilator routines
#include <verilated.h>

// Include Verilator model header, generated from Verilating "top.v"
#include "Vtop.h"

Vtop* top;   // Verilated model

vluint64_t main_time = 0;       // Current Verilator simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  You can also use a double, if you wish.
// One main_time unit corresponds to 0.1 seconds in real time

double sc_time_stamp () {       // Called by $time in Verilog
     return main_time;          // converts to double, to match what SystemC does
}

io_service io;
boost::posix_time::seconds interval(1);  // 1 second
deadline_timer timer(io, interval);

void tick(const boost::system::error_code& /*e*/) {

    main_time++;            // Verilator simulation time passes...

    top->clk_2 = !top->clk_2;       // Toggle clock

    top->eval();  // Evaluate Verilated SystemVerilog model

    // Reschedule the timer for 1 second in the future:
    timer.expires_at(timer.expires_at() + interval);
    // Posts the timer event
    timer.async_wait(tick);
}

// https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

//socket creation 
tcp::socket socket_(io);
//listener for new connection
tcp::acceptor *acceptor_;
boost::asio::streambuf sb;

void send_(tcp::socket & socket, const string& message) {
    const string msg = message + "\n";
    write( socket, buffer(message) );
}

void write_handle(const boost::system::error_code&, size_t);

void read_handle(const boost::system::error_code& err, size_t bytes_transferred)  {
    if (!err) {
         std::istream is(&sb);
         std::string line;
         std::getline(is, line);
         cout << line << endl;
         // assemble output string
         boost::asio::streambuf b;
         std::ostream sout(&b);
         sout << std::setfill('0') << std::hex << std::setw(2)
              << (int)top->SEG << std::setw(2) << (int)top->LED << endl;
         //write operation
         async_write(socket_, b, write_handle);
    } else {
         if (err != error::eof) {
             std::cerr << "read error: " << err.message() << std::endl;
         }
         socket_.close();

         // Destroy Verilog model
         delete top;

         // Fin
         exit(0);
    }
}

void write_handle(const boost::system::error_code& err, size_t bytes_transferred) {
    if (!err) {
       async_read_until(socket_, sb, '\n', read_handle);
    } else {
       std::cerr << "write error: " << err.message() << endl;
       socket_.close();
    }
}

void accept_handler(const boost::system::error_code& error)
{
  if (!error)
  {
    cout << "accepted" << endl;
    //read operation
    async_read_until(socket_, sb, '\n', read_handle);
  }
}

int main(int argc, char** argv, char** env) {

    if (argc < 2)
    {
      std::cerr << "Usage: " << argv[0] << " <port>\n";
      return 1;
    }

    // Construct the Verilated model, from Vtop.h generated from Verilating "top.v"
    top = new Vtop;

    top->SWI = 0;     // Initial settings of inputs
    top->clk_2 = 0;

    Verilated::commandArgs(argc, argv);   // Remember args

    //assign port number
    acceptor_ = new tcp::acceptor(io, tcp::endpoint(tcp::v4(), atoi(argv[1]) ));
 
    // Schedule the timer for the first time:
    timer.async_wait(tick);

    //waiting for connection
    acceptor_->async_accept(socket_, accept_handler);

    // Enter timer IO loop and never return.
    // The timer will fire for the first time 1 second from now.
    io.run();
}
