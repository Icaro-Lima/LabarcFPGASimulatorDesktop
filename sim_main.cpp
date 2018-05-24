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
SWI_Buttons *swi;  // switches
display *disp;     // LED, LCD, registers

vluint64_t main_time = 0;       // Current Verilator simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  You can also use a double, if you wish.
// One main_time unit corresponds to 0.1 seconds in real time

double sc_time_stamp () {       // Called by $time in Verilog
     return main_time;          // converts to double, to match what SystemC does
}

// set SystemVerilog input signal from graphic interface
void SWI_Buttons::toggle_cb(Fl_Widget *o, SWI_Buttons* this_o) { // this_o is this
  for(int i=NBUTTONS-1; i>=0; i--) {  // from most significant bit to least significant bit
    top->SWI <<= 1; // shift left, bit zero will get value zero
    top->SWI |= this_o->b[i]->value() & 1; // set bit zero from button
  }
}

void display::draw() {
  this->window()->make_current();  // needed because draw() will be called from callback

  // LEDs
  for(int i=0; i<8; i++)
    fl_rectf (this->x()+(7-i)*this->offset,this->y(),this->w(),this->h(),
              top->LED>>i & 1 ? FL_GREEN : FL_RED);

  fl_rectf(XMARGIN-2,80,385,270, FL_WHITE); // clean LCD and register window

  lcd_labels(95,25);
  // LCD data
  fl_font(FL_COURIER, 32);
  snprintf(str,STR_LENGTH,"%02X %08X %02X%1c%1c",
           top->lcd_pc, top->lcd_instruction, top->lcd_WriteData,
           top->lcd_MemWrite ? '*' : '_', top->lcd_Branch ? '*' : '_');
  fl_draw(str, XMARGIN, 125);
  snprintf(str,STR_LENGTH,"%02X %02X %02X %02X %02X%1c%1c",
           top->lcd_SrcA, top->lcd_SrcB, top->lcd_ALUResult, top->lcd_Result, top->lcd_ReadData,
           top->lcd_MemtoReg ? '*' : '_', top->lcd_RegWrite ? '*' : '_');
  fl_draw(str, XMARGIN, 165 );

  int y = 195;
  register_labels(y,18);
  // register values
  #define r top->lcd_registrador
  for(int i=0; i<32; i+=4) {
    snprintf(str,STR_LENGTH,"        : %02x      : %02x      : %02x      : %02x",
                            r[i], r[i+1], r[i+2], r[i+3]);
    fl_draw(str, XMARGIN, y += 18 );
  }
  #undef r
}

// ****** The main action is in this callback ******
// It controls Verilog simulation time, retrieves Verilog output and displays it.
void callback(void*) { 
  main_time++;            // Verilator simulation time passes...

  top->clk_2 = !top->clk_2;       // Toggle clock

  // Evaluate Verilated SystemVerilog model
  top->eval();

  // display SystemVerilog output in FLTK drawing
  leds->draw();

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

