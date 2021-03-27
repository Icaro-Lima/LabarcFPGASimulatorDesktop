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
#include <boost/process.hpp>

using namespace boost::asio;
using ip::tcp;
using boost::process::spawn;
using boost::system::error_code;
using ip::host_name;
using std::string;
using std::istream;
using std::ostream;
using std::cout;
using std::cerr;
using std::setfill;
using std::setw;
using std::setbase;
using std::hex;
using std::endl;
using std::__cxx11::stoi;
using std::__cxx11::to_string;

// Include common Verilator routines
#include <verilated.h>

// Include Verilator model header, generated from Verilating "top.v"
#include "Vtop.h"
#define NREGS 16

Vtop* top;   // Verilated model

vluint64_t main_time = 0;       // Current Verilator simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  You can also use a double, if you wish.
// One main_time unit corresponds to 0.1 seconds in real time

double sc_time_stamp () {       // Called by $time in Verilog
     return main_time;          // converts to double, to match what SystemC does
}

// service for timer and socket
io_service io;

boost::posix_time::seconds interval(1);  // 1 second
deadline_timer timer(io, interval); // timer for the clock signal

void tick(const error_code& ) {

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
tcp::socket sock(io);
// buffer for string received by socket
streambuf binp;

void write_handle(const error_code&, size_t); // prototype to be used in read_handle

// macro for SystemVerilog top module output
#define s(top_port)  setw(2) << (unsigned short)top->top_port

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
	 // decode command
         if ( (cmd & 0xF0) == 0x40) { // cmd = 0100xxxx - set/reset SWI
            unsigned short p =  1<<((cmd & 0xE)>>1); // bit position
            if( cmd&1 ) top->SWI |= p;  // set bit at position
            else        top->SWI &= ~p; // clear bit at position
            top->eval();  // Evaluate Verilated SystemVerilog model
         }
         // assemble output string
         streambuf bout;
         ostream sout(&bout);
         sout << setfill('0') << hex;
//  $cmd    # of bytes
//          returned       description
// -------------------------------------------------------------------
// 0000xxxx     16      RISC-V registers 0 to 15
// 00100011     12      RISC-V pc, instruction, SrcA, SrcB, ..., flags
// 0011xxxx     16      LCD 1st and 2nd line
// all others    2      LED and SEG
	 if ( (cmd & 0xF0) == 0x00) {  // cmd = 0000xxxx - RISC-V registers
            for (int i=0; i<NREGS; i++) sout << s(lcd_registrador[i]);
         } else if ( (cmd & 0xF0) == 0x30) {  // cmd = 0011xxxx - LCD
            sout << setw(16) << (unsigned long)top->lcd_b
                 << setw(16) << (unsigned long)top->lcd_a;
         } else if (cmd == 0x23) { // cmd = 001000011 - RISC-V pc, ...
            sout << s(lcd_pc) << setw(8) << (unsigned int)top->lcd_instruction
                 << s(lcd_SrcA) << s(lcd_SrcB) << s(lcd_ALUResult) << s(lcd_Result)
                 << s(lcd_WriteData) << s(lcd_ReadData)
                 << setw(2) << ( (top->lcd_RegWrite <<3) | (top->lcd_MemtoReg <<2) 
                               | (top->lcd_Branch   <<1) |  top->lcd_MemWrite     ); 
         } else { // all other cmd
            sout << s(SEG) << s(LED);
         }
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
         delete top;

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

    if (argc < 2)
    {
      cerr << "Usage: " << argv[0] << " <fpga>\n";
      return 1;
    }
    int fpga = atoi(argv[1]);  // FPGA number, starts from 0
    int port = fpga +2540;     // socket port number

    // Construct the Verilated model, from Vtop.h generated from Verilating "top.v"
    top = new Vtop;

    top->SWI = 0;     // Initial settings of inputs
    top->clk_2 = 0;

    Verilated::commandArgs(argc, argv);   // Remember args

    //listener for new connection
    tcp::acceptor acceptor_(io, tcp::endpoint(tcp::v4(), port ));
    //waiting for connection
    acceptor_.async_accept(sock, accept_handler);

    // Schedule the timer for the first time:
    timer.async_wait(tick);

#ifdef LAD
    cout << "<h4>Agora digite: ./remote "
         << host_name() << " " << fpga << " </h4>" << endl;
#else
    spawn("remote.bin", to_string(port));
#endif

    // Enter timer IO loop and never return.
    // The timer will fire for the first time 1 second from now.
    io.run();
}
