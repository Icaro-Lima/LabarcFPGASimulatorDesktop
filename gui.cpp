// Labarc FPGA Simulator
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2018

#include "gui.h"

SWI_Buttons::SWI_Buttons() {
  for(int i=0; i<NBUTTONS; i++) {
    label[i][0] = i+0x30; // map integer i to ASCII digit
    label[i][1] = 0;      // terminate label as string
    b[i] = new Fl_Toggle_Button(30+(NBUTTONS-i)*30,10,17,30,label[i]);
    b[i]->callback((Fl_Callback*)toggle_cb, this);
  }
}

LEDs::LEDs(int X,int Y,int W,int H) : Fl_Widget(X,Y,W,H) {}

void init_gui(int argc, char** argv) {
  window = new Fl_Window(400,200);  // window size 100 x 100 pixels

  swi = new SWI_Buttons();

  leds = new LEDs(0,0,100,100);

  window->end();
  window->show(argc,argv);

  Fl::add_timeout(0.25, callback);       // set up first timeout after 0.25 seconds
};

void delete_gui() {
    Fl::remove_timeout(callback);
    delete leds;
    delete swi;
    delete window;
}

