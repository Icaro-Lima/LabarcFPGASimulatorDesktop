#include <iostream>
#include <boost/asio.hpp>

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

boost::asio::io_service io_service;
boost::posix_time::seconds interval(1);  // 1 second
boost::asio::deadline_timer timer(io_service, interval);

void tick(const boost::system::error_code& /*e*/) {

    std::cout << "tick" << std::endl;

    // Reschedule the timer for 1 second in the future:
    timer.expires_at(timer.expires_at() + interval);
    // Posts the timer event
    timer.async_wait(tick);
}

int main(int argc, char** argv, char** env) {

    // Construct the Verilated model, from Vtop.h generated from Verilating "top.v"
    top = new Vtop;

    top->SWI = 0;     // Initial settings of inputs
    top->clk_2 = 0;

    Verilated::commandArgs(argc, argv);   // Remember args

    // Schedule the timer for the first time:
    timer.async_wait(tick);
    // Enter IO loop. The timer will fire for the first time 1 second from now:
    io_service.run();

    // Destroy Verilog model
    delete top;

    // Fin
    exit(0);
}
