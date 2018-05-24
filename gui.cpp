// Labarc FPGA Simulator
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2018

#include "gui.h"

SWI_Buttons::SWI_Buttons(int x, int y, int offset, int width, int height) {
  for(int i=0; i<NBUTTONS; i++) {
    label[i][0] = i+0x30; // map integer i to ASCII digit
    label[i][1] = 0;      // terminate label as string
    b[i] = new Fl_Toggle_Button(x + (NBUTTONS-i)*offset,y,width,height,label[i]);
    b[i]->callback((Fl_Callback*)toggle_cb, this);
  }
}

LEDs::LEDs(int x,int y,int offset,int width,int height) :
         Fl_Widget(x,y,width,height) {
           this->offset = offset;  // centered leds with respect to the x-axis
}

LEDs::LEDs(Fl_Window &window,int y, int offset, int width, int height) :
         Fl_Widget(window.decorated_w()/2-(7*(offset - width)+8*width)/2,y,width,height) { 
           this->offset = offset;  // centered leds with respect to the x-axis
         }

void init_gui(int argc, char** argv) {
  window = new Fl_Window(400,200);  // window size 100 x 100 pixels

  swi = new SWI_Buttons(30,10,30,17,30);

  leds = new LEDs(*window,50,20,10,20);

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

