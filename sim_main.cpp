// Labarc FPGA Simulator
// This file is derived from:
// verilator-3.922/examples/hello_world_c/sim_main.cpp
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2018

// Include graphic user interface
#include "gui.h"

// Include common Verilator routines
#include <verilated.h>

// Include Verilator model header, generated from Verilating "top.v"
#include "Vtop.h"

Vtop* top;   // Verilated model
Fl_Window *window; // Window representing FPGA board
SWIs *swi;  // switches
LEDs *leds;
display *disp;     // LED, LCD, registers
SegmentsDisplay *segments; // 7-segment display

vluint64_t main_time = 0;       // Current Verilator simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  You can also use a double, if you wish.
// One main_time unit corresponds to 0.1 seconds in real time

double sc_time_stamp () {       // Called by $time in Verilog
     return main_time;          // converts to double, to match what SystemC does
}

int SWI::handle(int event) {
	if (event == FL_PUSH) {
		state = !state;
		
		if (state) {
			top->SWI |= 1UL << id;
		} else {
			top->SWI &= ~(1UL << id);
		}
	}
}

void SWI::draw() {
	(state ? swi_on : swi_off)->draw(x, y);
}

void SWIs::draw() {
	for  (int i = 0; i < 8; i++) {
		swis[i]->draw();
	}
}

void LEDs::draw() {	
	for(int i = 0; i < 8; i++) {
		((top->LED >> i & 1) ? led_on : led_off)->draw(x_origin + (7 - i) * offset, y_origin);
	}
}

SegmentsDisplay::SegmentsDisplay(int x_origin, int y_origin) {
	this->x_origin = x_origin;
	this->y_origin = y_origin;
	
	this->previous = new bool[8];
	memset(this->previous, 0, 8 * sizeof(bool));
	
	base = new Fl_PNG_Image("Assets/7SegmentsDisplayBase.png");
	point_on = new Fl_PNG_Image("Assets/PointOn.png");
	point_off = new Fl_PNG_Image("Assets/PointOff.png");
	vertical_on = new Fl_PNG_Image("Assets/VerticalOn.png");
	vertical_off = new Fl_PNG_Image("Assets/VerticalOff.png");
	horizontal_on = new Fl_PNG_Image("Assets/HorizontalOn.png");
	horizontal_off = new Fl_PNG_Image("Assets/HorizontalOff.png");
}

void SegmentsDisplay::draw() {			
	base->draw(x_origin, y_origin);
	
	(top->SEG>>0 & 1 ? horizontal_on : horizontal_off)->draw(x_origin + 26, y_origin + 25);
	(top->SEG>>1 & 1 ? vertical_on : vertical_off)->draw(x_origin + 74, y_origin + 33);
	(top->SEG>>2 & 1 ? vertical_on : vertical_off)->draw(x_origin + 74, y_origin + 88);
	(top->SEG>>3 & 1 ? horizontal_on : horizontal_off)->draw(x_origin + 26, y_origin + 135);
	(top->SEG>>4 & 1 ? vertical_on : vertical_off)->draw(x_origin + 17, y_origin + 88);
	(top->SEG>>5 & 1 ? vertical_on : vertical_off)->draw(x_origin + 17, y_origin + 33);
	(top->SEG>>6 & 1 ? horizontal_on : horizontal_off)->draw(x_origin + 26, y_origin + 80);
	(top->SEG>>7 & 1 ? point_on : point_off)->draw(x_origin + 91, y_origin + 134);
}

void display::draw() {
  this->window()->make_current();  // needed because draw() will be called from callback

  fl_rectf(this->x() + XMARGIN - 2, 80, 385, 270, FL_WHITE); // clean LCD and register window

  lcd_labels(95, 25);
  stringstream ss;
  ss << hex << setfill('0') << uppercase;
  // LCD data
  // first line
  ss << setw(2) << (int)top->lcd_pc;
  ss << " " << setw(8) << (int)top->lcd_instruction;
  ss << " " << setw(2) << (int)top->lcd_WriteData;
  ss << (top->lcd_MemWrite ? '*' : '_');
  ss << (top->lcd_Branch ? '*' : '_');
  fl_draw(ss.str().c_str(), this->x() + XMARGIN, 125);
	
  // second line
  ss.str(""); // reset stringstream
  ss << setw(2) << (int)top->lcd_SrcA;
  ss << " " << setw(2) << (int)top->lcd_SrcB;
  ss << " " << setw(2) << (int)top->lcd_ALUResult;
  ss << " " << setw(2) << (int)top->lcd_Result;
  ss << " " << setw(2) << (int)top->lcd_ReadData;
  ss << (top->lcd_MemtoReg ? '*' : '_');
  ss << (top->lcd_RegWrite ? '*' : '_');
  fl_draw(ss.str().c_str(), this->x() + XMARGIN, 165);
  
  int y = 195;
  register_labels(y, 18);
  // register values
  ss << nouppercase;
  for(int i = 0; i < 32; i++) { //  for all registradores
    if(i % 4 == 0) {  // start of line
      ss.str(""); // reset stringstream
      ss << "  ";
    }
    ss << "      : " << setw(2) << (int)top->lcd_registrador[i];
    if(i % 4 == 3) { // end of line
      fl_draw(ss.str().c_str(), this->x() + XMARGIN, y += 18);
    }
  }
}

// ****** The main action is in this callback ******
// It controls Verilog simulation time, retrieves Verilog output and displays it.
void callback(void*) { 
  main_time++;            // Verilator simulation time passes...

  top->clk_2 = !top->clk_2;       // Toggle clock

  // Evaluate Verilated SystemVerilog model
  top->eval();

  leds->draw();

  swi->draw();

  disp->draw();
  
  segments->draw();
	
  Fl::repeat_timeout(0.25, callback);    // retrigger timeout after 0.1 seconds
}

int main(int argc, char** argv, char** env) {
    init_gui(argc,argv);

    Verilated::commandArgs(argc, argv);   // Remember args

    // Construct the Verilated model, from Vtop.h generated from Verilating "top.v"
    top = new Vtop;

    top->SWI = 0;     // Initial settings of inputs
    top->clk_2 = 0;

    Fl::run();   // run the graphical interface which calls callback()

    // Final Verilog model cleanup
    top->final();
    // Destroy Verilog model
    delete top;

    delete_gui;

    // Fin
    exit(0);
}

