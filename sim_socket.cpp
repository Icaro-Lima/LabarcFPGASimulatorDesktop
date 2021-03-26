#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
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

io_service io_tick;
boost::posix_time::seconds interval(1);  // 1 second
deadline_timer timer(io_tick, interval);

void tick(const boost::system::error_code& /*e*/) {

    cout << "tick" << endl;

    // Reschedule the timer for 1 second in the future:
    timer.expires_at(timer.expires_at() + interval);
    // Posts the timer event
    timer.async_wait(tick);
}

string read_(tcp::socket & socket) {
       streambuf buf;
       read_until( socket, buf, "\n" );
       string data = buffer_cast<const char*>(buf.data());
       return data;
}

void send_(tcp::socket & socket, const string& message) {
       const string msg = message + "\n";
       write( socket, buffer(message) );
}

int main(int argc, char** argv, char** env) {

    // Construct the Verilated model, from Vtop.h generated from Verilating "top.v"
    top = new Vtop;

    top->SWI = 0;     // Initial settings of inputs
    top->clk_2 = 0;

    Verilated::commandArgs(argc, argv);   // Remember args

    io_service io_socket;
    //listen for new connection
    tcp::acceptor acceptor_(io_socket, tcp::endpoint(tcp::v4(), 2540 ));
    //socket creation 
    tcp::socket socket_(io_socket);
    //waiting for connection
    acceptor_.accept(socket_);
    // now, in a terminal window:  telnet localhost 2540

    // Schedule the timer for the first time:
    timer.async_wait(tick);
    // Enter IO loop. The timer will fire for the first time 1 second from now:
    io_tick.run();

    // Destroy Verilog model
    delete top;

    // Fin
    exit(0);
}
