// Labarc remote FPGA display
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2021

// g++ -std=c++11 remote.cpp gui.o -o remote.bin \
//     -lboost_system -lpthread -lfltk_images -lpng -lz -lfltk

// This program is based in part on the work of the FLTK project (http://www.fltk.org) and
// https://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/echo/blocking_tcp_echo_client.cpp

// Include graphic user interface
#include "gui.h"

#include <bitset>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

struct top_struct
   { unsigned char SWI, LED, SEG,
                   lcd_pc, lcd_SrcA, lcd_SrcB, lcd_ALUResult, lcd_Result,
                   lcd_ReadData, lcd_WriteData, lcd_registrador[32];
     unsigned int lcd_instruction;
     bool lcd_RegWrite, lcd_MemWrite, lcd_MemtoReg, lcd_Branch;
     unsigned long lcd_a, lcd_b; } top_s;

struct top_struct *top = &top_s;

boost::asio::io_service io_service;

tcp::socket sock(io_service);
tcp::resolver resolver(io_service);

enum { max_length = 10 };

void set_led_seg(char *reply) {
    unsigned int r = strtol(reply, NULL, 16);  // convert from hex
    top->LED = r & 0xFF;
    top->SEG = r >>8;
}

void send_and_set(string r) {
  try
  {
    const char *request = r.c_str();
    size_t request_length = std::strlen(request);
    boost::asio::write(sock, boost::asio::buffer(request, request_length));

    char reply[max_length];
    size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, 4));
    reply[reply_length] = 0; // trim
    set_led_seg(reply);
    // receive CR and LF
    reply_length = boost::asio::read(sock, boost::asio::buffer(reply, 2));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
    exit(9);
  }
}

// ****** The main action is in this callback ******
void callback(void*) {

  string r;

  send_and_set("00100000\n");

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

       send_and_set("0100" + std::bitset<3>(id).to_string() + (state?"1":"0") + "\n");
		
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
    if(argc==2) {
       host = (char *)"localhost";
       port = argv[1];
       init_gui(argc-1,argv+1); // dirty argv[0] :-(
    } else {
       host = argv[1];
       port = argv[2];
       init_gui(argc-2,argv+2); // dirty argv[0] :-(
    }   

  try
  {
    boost::asio::connect(sock, resolver.resolve({host, port}));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

    Fl::run();   // run the graphical interface which calls callback()

    delete_gui();

    // Fin
    exit(0);
}

