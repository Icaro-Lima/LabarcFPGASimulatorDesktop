// Labarc FPGA Simulator
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2018

#include "gui.h"

SWI::SWI(int x, int y, int id, Fl_PNG_Image *swi_on, Fl_PNG_Image *swi_off) : 
	x(x), 
	y(y), 
	id(id), 
	state(false), 
	swi_on(swi_on), 
	swi_off(swi_off), 
	Fl_Widget(x, y, 33, 96) { }
	
SWIs::SWIs(int x, int y, int offset) : swi_on(new Fl_PNG_Image("Assets/SWIOn.png")), swi_off(new Fl_PNG_Image("Assets/SWIOff.png")) {
	for (int i = 0; i < 8; i++) {
		swis[i] = new SWI(x + offset * (7 - i), y, i, swi_on, swi_off);
	}
}

LEDs::LEDs(int x_origin, int y_origin, int offset) {
	led_on = new Fl_PNG_Image("Assets/LEDOn.png");
	led_off = new Fl_PNG_Image("Assets/LEDOff.png");
		
	this->x_origin = x_origin;
	this->y_origin = y_origin;
	this->offset = offset;
}

display::display(int x, int y, int width, int height) : Fl_Widget(x, y, width, height) { }

void display::lcd_labels(int start, int step) {
  fl_font(DISPLAY_FONT, 13);
  fl_color(FL_RED);
  fl_draw("  pc       instruction     WriteData MemWrite", this->x() + XMARGIN, start);
  fl_draw("Branch", this->x() + 320, start+step);
  fl_draw("SrcA SrcB ALUResult Result ReadData MemtoReg", this->x() + XMARGIN, start + 3.5 * step);
  fl_draw("RegWrite",this->x() + 320, start + 2.5 * step);
  fl_color(FL_BLACK);
  fl_font(DISPLAY_FONT, 32);
};

void display::register_labels(int start, int step) {
  int y = start;
  fl_color(FL_RED);
  fl_font(DISPLAY_FONT, 13);
  fl_draw("x0  zero      ra        sp        gp ", this->x() + XMARGIN, y += step );
  fl_draw("x4  tp        t0        t1        t2 ", this->x() + XMARGIN, y += step );
  fl_draw("x8  s0        s1        a0        a1 ", this->x() + XMARGIN, y += step );
  fl_draw("x12 a2        a3        a4        a5 ", this->x() + XMARGIN, y += step );
  fl_draw("x16 a6        a7        s2        s3 ", this->x() + XMARGIN, y += step );
  fl_draw("x20 s4        s5        s6        s7 ", this->x() + XMARGIN, y += step );
  fl_draw("x24 s8        s9        s10       s11", this->x() + XMARGIN, y += step );
  fl_draw("x28 t3        t4        t5        t6 ", this->x() + XMARGIN, y += step );
  fl_color(FL_BLACK);
}

const char *mono_fonts[] = { "Lucida Console",
                             "Droid Sans Mono",
                             "Noto Mono",
                             "Monospace",
                             "Consolas",
                             "" };

void init_gui(int argc, char** argv) {
  int window_width = 800;
  int window_height = 500;
  window = new Fl_Window(Fl::w() / 2 - window_width / 2, Fl::h() / 2 - window_height / 2, window_width, window_height, "Labarc FPGA Simulator");

  swi = new SWIs(0, 400, 40);
  
  leds = new LEDs(55, 30, 30);

  int i=0;
  do {  // search for an existin mono-space font
    Fl::set_font(DISPLAY_FONT, mono_fonts[i++]);
    fl_font(DISPLAY_FONT, 13);
  } while( fl_width('w') != fl_width('i') && strlen(mono_fonts[i]) );
    
  disp = new display(200, 20, 10, 20);
  
  segments = new SegmentsDisplay(600, 0);

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

