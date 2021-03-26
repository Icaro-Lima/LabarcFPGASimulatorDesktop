#include <iostream>
#include <iomanip>
#include <boost/asio.hpp>

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

io_service io_tick;
boost::posix_time::seconds interval(1);  // 1 second
deadline_timer timer(io_tick, interval);

void tick(const boost::system::error_code& /*e*/) {

    cout << "tick" << endl;
    main_time++;            // Verilator simulation time passes...

    top->clk_2 = !top->clk_2;       // Toggle clock

    top->eval();  // Evaluate Verilated SystemVerilog model

    // Reschedule the timer for 1 second in the future:
    timer.expires_at(timer.expires_at() + interval);
    // Posts the timer event
    timer.async_wait(tick);
}

// https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

io_service io_socket;
//socket creation 
tcp::socket socket_(io_socket);
//listener for new connection
tcp::acceptor *acceptor_;

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

void socket_run() {
    //waiting for connection
    acceptor_->accept(socket_);
    //read operation
    string message = read_(socket_);
    cout << message << endl;
    // assemble output string
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << std::hex << (int)top->LED << endl;
    //write operation
    send_(socket_, stream.str());    
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

    //assign socket port number
    acceptor_ = new tcp::acceptor(io_socket, tcp::endpoint(tcp::v4(), atoi(argv[1]) ));

    // Schedule the timer for the first time:
    timer.async_wait(tick);

    int pid;
    // Enter timer IO loop and never return. The timer will fire for the first time 1 second from now.
    if ( (pid=fork()) ==0) io_tick.run();
    else  socket_run();

    kill(pid, SIGKILL); // kill timer

    // Destroy Verilog model
    delete top;

    cout << "Exit" << endl;
    // Fin
    exit(0);
}
