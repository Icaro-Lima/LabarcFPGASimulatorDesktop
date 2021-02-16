// Labarc remote FPGA display
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2021

// This program is based in part on the work of the FLTK project (http://www.fltk.org) and
// https://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/echo/blocking_tcp_echo_client.cpp

// Include graphic user interface
#include "gui.h"

#include <bitset>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

// declare inputs and outputs of module top as struct,
// similar to what Verilator does automatically
struct top_struct
   { unsigned char SWI, LED, SEG,
                   lcd_pc, lcd_SrcA, lcd_SrcB, lcd_ALUResult, lcd_Result,
                   lcd_ReadData, lcd_WriteData, lcd_registrador[32];
     unsigned int lcd_instruction;
     bool lcd_RegWrite, lcd_MemWrite, lcd_MemtoReg, lcd_Branch;
     unsigned long lcd_a, lcd_b; } top_s;

struct top_struct *top = &top_s;

// declare socket and auxiliary elements for remote connection
using boost::asio::ip::tcp;
boost::asio::io_service io_service;
tcp::socket sock(io_service);
tcp::resolver resolver(io_service);
enum { max_length = 10,
       expected_reply_length = 4 };

// copy information received from remote FPGA to outputs of module top
void set_led_seg(char *reply) {
    unsigned int r = strtol(reply, NULL, 16);  // convert hex string to int
    top->LED = r & 0xFF; // least significant byte is LED
    top->SEG = r >>8;    // second least significant byte is seven segment display
}

// send a request string and treat the reply
void send_and_set(string r) {
  try
  {
    // send request string - typically 8 characters representing a binary number
    const char *request = r.c_str();
    size_t request_length = std::strlen(request);
    boost::asio::write(sock, boost::asio::buffer(request, request_length));

    // reveive a string of 4 characters representing a hex number 
    char reply[max_length];
    size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, expected_reply_length));
    reply[reply_length] = 0; // trim
    set_led_seg(reply);
    // receive CR and LF (exactly 2 characters) and discard them
    reply_length = boost::asio::read(sock, boost::asio::buffer(reply, 2));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    exit(9); // exit in case of connection error
  }
}

// ****** The main action is in this callback ******
void callback(void*) {

  string r;

  send_and_set("00100000\n");  // periodically request LED an SEG state

  fpga->redraw();
    	
  Fl::repeat_timeout(fpga->clk->value()/2, callback);    // retrigger timeout for next clock change
}

int SWI::handle(int event) {
	if (event == FL_PUSH) {
		state = !state;
		
		if (state) {
			top->SWI |= 1UL << id;
		} else {
			top->SWI &= ~(1UL << id);
		}

	// send a string representing 8 bits as command to modify SWI
	// the higher nibble is 0100
	// the lower nibble consists of 3 bits for bit position
	// and the least significant bit is the value to be assigned to that position
	send_and_set("0100" + std::bitset<3>(id).to_string() + (state?"1":"0") + "\n");
	// this call also updates LED and SEG
		
       fpga->redraw();
    }
    return 1;
}

void LEDs::draw() {	
	for(int i = 0; i < NLEDS; i++) {
		((top->LED >> i & 1) ? led_on : led_off)->draw(x_origin + (NLEDS-1 - i) * offset, y_origin);
	}
}

void SegmentsDisplay::draw() {	
	base->draw(x(), y());
	draw_segments(top->SEG);
}

void display::draw() {
  this->window()->make_current();  // needed because draw() will be called from callback

  fl_rectf(x(), y(), w(), h(), FL_WHITE); // clean LCD and register window

  lcd_labels();
  stringstream ss;
  ss << hex << setfill('0') << uppercase;
  // LCD data
  // first line
  ss << setw(2) << (int)top->lcd_pc;
  ss << " " << setw(8) << (int)top->lcd_instruction;
  ss << " " << setw(2) << (int)top->lcd_WriteData;
  ss << (top->lcd_MemWrite ? '*' : '_');
  ss << (top->lcd_Branch ? '*' : '_');
  fl_draw(ss.str().c_str(), x() + XMARGIN, y() + DISPLAY_FONT_SIZE+LCD_FONT_SIZE);
	
  // second line
  ss.str(""); // reset stringstream
  ss << setw(2) << (int)top->lcd_SrcA;
  ss << " " << setw(2) << (int)top->lcd_SrcB;
  ss << " " << setw(2) << (int)top->lcd_ALUResult;
  ss << " " << setw(2) << (int)top->lcd_Result;
  ss << " " << setw(2) << (int)top->lcd_ReadData;
  ss << (top->lcd_MemtoReg ? '*' : '_');
  ss << (top->lcd_RegWrite ? '*' : '_');
  fl_draw(ss.str().c_str(), this->x() + XMARGIN, y() + DISPLAY_FONT_SIZE+2*LCD_FONT_SIZE);
  
  register_labels();
  int yy = y() + 3*DISPLAY_FONT_SIZE + 2*LCD_FONT_SIZE;
  // register values
  ss << nouppercase;
  for(int i = 0; i < NREGS; i++) { //  for all registradores
    if(i % NREGS_PER_LINE == 0) {  // start of line
      ss.str(""); // reset stringstream
      ss << "  ";
    }
    ss << "      : " << setw(2) << (int)top->lcd_registrador[i];
    if(i % NREGS_PER_LINE == NREGS_PER_LINE-1) { // end of line
      fl_draw(ss.str().c_str(), this->x() + XMARGIN, yy += 1.5*DISPLAY_FONT_SIZE);
    }
  }
}

void hexval::draw() {
  this->window()->make_current();  // needed because draw() will be called from callback

  fl_rectf(x(), y(), w(), h(), FL_WHITE); // clean LCD window

  lcd_lines((long)top->lcd_a, (long)top->lcd_b);
}

int main(int argc, char** argv, char** env) {

    if (argc < 2)
    {
      std::cerr << "Usage: " << argv[0] << " [<host>] <port>\n";
      return 1;
    }

    char *host;
    char *port;
    int argc_offset;
    if(argc==2) {  // default for host is localhost
       host = (char *)"localhost";
       port = argv[1];
       argc_offset = 1;
    } else {
       host = argv[1];
       port = argv[2];
       argc_offset = 2;
    }   

  try
  {
    boost::asio::connect(sock, resolver.resolve({host, port}));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    exit(8); // exit in case of connection error
  }

    init_gui(argc-argc_offset,argv+argc_offset); // dirty argv[0] :-(

    Fl::run();   // run the graphical interface which calls callback()

    delete_gui();

    // Fin
    exit(0);
}

