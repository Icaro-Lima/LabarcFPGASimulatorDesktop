// Labarc FPGA Simulator
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2018

#include "gui.h"

SWI_Buttons::SWI_Buttons(int x, int y, int offset, int individual_width, int individual_height) {
  for(int i=0; i<NBUTTONS; i++) {
    label[i][0] = i+0x30; // map integer i to ASCII digit
    label[i][1] = 0;      // terminate label as string
    b[i] = new Fl_Toggle_Button(x + (NBUTTONS-i)*offset,y,individual_width,individual_height,label[i]);
    b[i]->callback((Fl_Callback*)toggle_cb, this);
  }
}

LEDs::LEDs(int x_origin, int y_origin, int offset) {
	led_on = new Fl_PNG_Image("Assets/LEDOn.png");
	led_off = new Fl_PNG_Image("Assets/LEDOff.png");
	
	this->states = new bool[8];
	
	this->x_origin = x_origin;
	this->y_origin = y_origin;
	this->offset = offset;
}

void LEDs::draw() {
	for(int i = 0; i < 8; i++) {
		(states[i] ? led_on : led_off)->draw(x_origin + (7 - i) * offset, y_origin);
	}
}

display::display(int x,int y,int offset,int width,int height) :
         Fl_Widget(x,y,width,height) {
           this->offset = offset;  // centered leds with respect to the x-axis
         }

display::display(Fl_Window *window,int y, int offset, int width, int height) :
         Fl_Widget(window->decorated_w()/2-(7*(offset - width)+8*width)/2,y,width,height) { 
           this->offset = offset;  // centered leds with respect to the x-axis
         }

void display::lcd_labels(int start, int step) {
  fl_font(DISPLAY_FONT, 13);
  fl_color(FL_RED);
  fl_draw("  pc       instruction     WriteData MemWrite", XMARGIN, start );
  fl_draw("Branch",320,start+step);
  fl_draw("SrcA SrcB ALUResult Result ReadData MemtoReg", XMARGIN, start+3.5*step );
  fl_draw("RegWrite",320,start+2.5*step);
  fl_color(FL_BLACK);
  fl_font(DISPLAY_FONT, 32);
};

void display::register_labels(int start, int step) {
  int y = start;
  fl_color(FL_RED);
  fl_font(DISPLAY_FONT, 13);
  fl_draw("x0  zero      ra        sp        gp ", XMARGIN, y += step );
  fl_draw("x4  tp        t0        t1        t2 ", XMARGIN, y += step );
  fl_draw("x8  s0        s1        a0        a1 ", XMARGIN, y += step );
  fl_draw("x12 a2        a3        a4        a5 ", XMARGIN, y += step );
  fl_draw("x16 a6        a7        s2        s3 ", XMARGIN, y += step );
  fl_draw("x20 s4        s5        s6        s7 ", XMARGIN, y += step );
  fl_draw("x24 s8        s9        s10       s11", XMARGIN, y += step );
  fl_draw("x28 t3        t4        t5        t6 ", XMARGIN, y += step );
  fl_color(FL_BLACK);
}

const char *mono_fonts[] = { "Lucida Console",
                             "Droid Sans Mono",
                             "Noto Mono",
                             "Monospace",
                             "Consolas",
                             "" };

void init_gui(int argc, char** argv) {
  window = new Fl_Window(600, 360, "Labarc FPGA Simulator");

  swi = new SWI_Buttons(30,10,30,17,30);
  
  leds = new LEDs(55, 40, 30);

  int i=0;
  do {  // search for an existin mono-space font
    Fl::set_font(DISPLAY_FONT, mono_fonts[i++]);
    fl_font(DISPLAY_FONT, 13);
  } while( fl_width('w') != fl_width('i') && strlen(mono_fonts[i]) );
    
  disp = new display(window,50,20,10,20);
  
  segments = new SegmentsDisplay();

  window->end();
  window->show(argc,argv);

  Fl::add_timeout(0.25, callback);       // set up first timeout after 0.25 seconds
};

void delete_gui() {
    Fl::remove_timeout(callback);
    delete disp;
    delete swi;
    delete window;
}

