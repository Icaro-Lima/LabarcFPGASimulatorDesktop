// Labarc FPGA Simulator
// This file is derived from:
// verilator-3.922/examples/hello_world_c/sim_main.cpp
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2019

// Include graphic user interface
#include "gui.h"

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

// ****** The main action is in this callback ******
// It controls Verilog simulation time, retrieves Verilog output and displays it.
void callback(void*) { 
  main_time++;            // Verilator simulation time passes...

  top->clk_2 = !top->clk_2;       // Toggle clock

  top->eval();  // Evaluate Verilated SystemVerilog model
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
		
		top->eval();  // Evaluate Verilated SystemVerilog model
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

    delete_gui();

    // Fin
    exit(0);
}

