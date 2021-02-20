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
                   lcd_ReadData, lcd_WriteData, lcd_registrador[NREGS];
     unsigned int lcd_instruction;
     bool lcd_RegWrite, lcd_MemWrite, lcd_MemtoReg, lcd_Branch;
     unsigned long lcd_a, lcd_b; } top_s;

struct top_struct *top = &top_s;

// declare socket and auxiliary elements for remote connection
using boost::asio::ip::tcp;
boost::asio::io_service io_service;
tcp::socket sock(io_service);
tcp::resolver resolver(io_service);
enum { max_length = 40 };

// send a request string and receive the reply
char *send_and_rec(string r, int expected_reply_length) {
  static char reply[max_length];
  try
  {
    // send request string - typically 8 characters representing a binary number
    const char *request = r.c_str();
    size_t request_length = std::strlen(request);
    boost::asio::write(sock, boost::asio::buffer(request, request_length));

    // reveive a string of characters representing a hex number 
    size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, expected_reply_length));
    reply[reply_length] = 0; // trim
    // receive CR and LF (exactly 2 characters) and discard them
    char crlf[4];
    reply_length = boost::asio::read(sock, boost::asio::buffer(crlf, 2));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    exit(9); // exit in case of connection error
  }
  return reply;
}

// copy information received from remote FPGA to outputs of module top
void set_led_seg(char *reply) {
    unsigned int r = strtoul(reply, NULL, 16);  // convert hex string to int
    top->LED = r & 0xFF; // least significant byte is LED
    top->SEG = r >>8;    // second least significant byte is seven segment display
}

void set_lcd_ab(char *reply) {
    top->lcd_a = strtoul(reply+16, NULL, 16);      // lcd_a are the rightmost 16 characters
    reply[16] = 0;                                 // cut off lcd_a (reply string gets proposedly damaged)
    top->lcd_b = strtoul(reply, NULL, 16);         // lcd_b are the first 16 characters
}

void set_pc_etc(char *reply) {
    unsigned char flags  = strtoul(reply+22, NULL, 16); reply[22] = 0;
    top->lcd_MemWrite    = (flags & 1)>0;
    top->lcd_Branch      = (flags & 2)>0;
    top->lcd_MemtoReg    = (flags & 4)>0;
    top->lcd_RegWrite    = (flags & 8)>0;
    top->lcd_ReadData    = strtoul(reply+20, NULL, 16); reply[20] = 0;
    top->lcd_WriteData   = strtoul(reply+18, NULL, 16); reply[18] = 0;
    top->lcd_Result      = strtoul(reply+16, NULL, 16); reply[16] = 0;
    top->lcd_ALUResult   = strtoul(reply+14, NULL, 16); reply[14] = 0;
    top->lcd_SrcB        = strtoul(reply+12, NULL, 16); reply[12] = 0;
    top->lcd_SrcA        = strtoul(reply+10, NULL, 16); reply[10] = 0;
    top->lcd_instruction = strtoul(reply+ 2, NULL, 16); reply[ 2] = 0;
    top->lcd_pc          = strtoul(reply, NULL, 16);
}

void set_regs(char *reply) {
   for(int i=NREGS-1; i>=0; i--) {
      top->lcd_registrador[i] = strtoul(reply+2*i, NULL, 16);
      reply[2*i] = 0;
   }
}

void rec_set_lcd() {
   if(fpga->lcd_check->value()) set_lcd_ab( send_and_rec("00111111\n", 32) );
   else if(fpga->riscv_check->value()) {
         set_pc_etc( send_and_rec("00100010\n", 24) );
         set_regs( send_and_rec("00000000\n", 32) );
   }
}

// ****** The main action is in this callback ******
void callback(void*) {

  string r;

  set_led_seg( send_and_rec("00100000\n", 4) );  // request LED and SEG state
  rec_set_lcd();

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
	set_led_seg( send_and_rec("0100" + std::bitset<3>(id).to_string() + (state?"1":"0") + "\n", 4) );
	// this call also updates LED and SEG
		
        rec_set_lcd();

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

  if(fpga->riscv_check->value()) {
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
}

void hexval::draw() {
  this->window()->make_current();  // needed because draw() will be called from callback

  if(fpga->lcd_check->value()) {
     fl_rectf(x(), y(), w(), h(), FL_WHITE); // clean LCD window

     lcd_lines((long)top->lcd_a, (long)top->lcd_b);
  }
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

    init_gui(argc-argc_offset,argv+argc_offset, (char *)"Remote FPGA board"); // dirty argv[0] :-(

    Fl::run();   // run the graphical interface which calls callback()

    delete_gui();

    // Fin
    exit(0);
}

