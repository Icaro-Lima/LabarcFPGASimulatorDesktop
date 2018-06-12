// Labarc FPGA Simulator
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2018

#include "gui.h"

Fl_PNG_Image * FPGA::image = new Fl_PNG_Image("Assets/FPGA.png");
FPGA::FPGA(int x, int y) : Fl_Widget(x, y, image->w(), image->h()) {
	disp = new display(x + image->w() / 2, y + image->h() / 2);
	disp->position(x + image->w() / 2 - disp->w() / 2, y + image->h() / 2);
	
	board = new Board(x + image->w() / 2 - board->image->w() / 2, y);
	
	int led_offset = 30;
	int led_x = x + image->w() / 2 - (leds->led_on->w() * 8 + 7 * (led_offset - leds->led_on->w())) / 2;
	leds = new LEDs(led_x, y + LEDS_VERTICAL_OFFSET, led_offset);
}

void FPGA::draw() {
	if (damage() == 0x80) {
		image->draw(x(), y());
	}
	if (damage() & 1) {
		leds->redraw();
		board->damage(1);
		disp->redraw();
	}
}

Fl_PNG_Image * Board::image = new Fl_PNG_Image("Assets/Board.png");
Board::Board(int x, int y) : 
	swis(new SWIs(x + image->w() / 2 - (8 * SWIs::swi_on->w() + 7 * (SWIS_OFFSET - SWIs::swi_on->w()) + SegmentsDisplay::base->w()) / 2,
	y + image->h() / 2 - SWIs::swi_on->h() / 2, SWIS_OFFSET)), 
	segments(new SegmentsDisplay(x + image->w() / 2 - segments->base->w() / 2, y + image->h() / 2 - segments->base->h() / 2)), 
	Fl_Widget(x, y, image->w(), image->h()) {
		
	}

void Board::draw() {
	if (damage() & 0x80) {
		image->draw(x(), y());
		swis->redraw();
	}
	if (damage() & 1) {
		image->draw(swis->x(), swis->y(), swis->w(), swis->h(), x() + swis->x(), y() + swis->y());
		swis->redraw();
		image->draw(segments->x(), segments->y(), segments->w(), segments->h(), x() + segments->x(), y() + segments->y());
		segments->redraw();
	}
}

SWI::SWI(int x, int y, int id, Fl_PNG_Image *swi_on, Fl_PNG_Image *swi_off) : 
	id(id), 
	state(false), 
	swi_on(swi_on),
	swi_off(swi_off), 
	Fl_Widget(x, y, 33, 96) { }

Fl_PNG_Image * SWIs::swi_on = new Fl_PNG_Image("Assets/SWIOn.png");
Fl_PNG_Image * SWIs::swi_off = new Fl_PNG_Image("Assets/SWIOff.png");
SWIs::SWIs(int x, int y, int offset) : Fl_Widget(x, y, 8 * 33 + 7 * (offset - 33) + SegmentsDisplay::base->w(), 96) {
	for (int i = 0; i < 4; i++) {
		swis[i] = new SWI(SegmentsDisplay::base->w() + x + offset * (7 - i), y, i, swi_on, swi_off);
	}
	
	for (int i = 4; i < 8; i++) {
		swis[i] = new SWI(x + offset * (7 - i), y, i, swi_on, swi_off);
	}
}

Fl_PNG_Image * LEDs::led_on = new Fl_PNG_Image("Assets/LEDOn.png");
Fl_PNG_Image * LEDs::led_off = new Fl_PNG_Image("Assets/LEDOff.png");
LEDs::LEDs(int x_origin, int y_origin, int offset) : Fl_Widget(x_origin, y_origin, 8 * led_on->w() + 7 * (offset - led_on->w()), led_on->h()) {
	this->x_origin = x_origin;
	this->y_origin = y_origin;
	this->offset = offset;
}

Fl_PNG_Image * SegmentsDisplay::base = new Fl_PNG_Image("Assets/7SegmentsDisplayBase.png");
SegmentsDisplay::SegmentsDisplay(int x, int y) : Fl_Widget(x, y, base->w(), base->h()) {
	this->previous = new bool[8];
	memset(this->previous, 0, 8 * sizeof(bool));
	
	point_on = new Fl_PNG_Image("Assets/PointOn.png");
	point_off = new Fl_PNG_Image("Assets/PointOff.png");
	vertical_on = new Fl_PNG_Image("Assets/VerticalOn.png");
	vertical_off = new Fl_PNG_Image("Assets/VerticalOff.png");
	horizontal_on = new Fl_PNG_Image("Assets/HorizontalOn.png");
	horizontal_off = new Fl_PNG_Image("Assets/HorizontalOff.png");
}

display::display(int x, int y) : Fl_Widget(x, y, 390, 270) { }

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
	int window_width = FPGA::image->w();
	int window_height = FPGA::image->h();;
	window = new Fl_Window(Fl::w() / 2 - window_width / 2, Fl::h() / 2 - window_height / 2, window_width, window_height, "Labarc FPGA Simulator");

	fpga = new FPGA(0, 0);
	  
	int i=0;
	do {  // search for an existin mono-space font
	  Fl::set_font(DISPLAY_FONT, mono_fonts[i++]);
	  fl_font(DISPLAY_FONT, 13);
	} while( fl_width('w') != fl_width('i') && strlen(mono_fonts[i]) );
			  
	window->end();
	window->show(argc,argv);

	Fl::add_timeout(0.25, callback);       // set up first timeout after 0.25 seconds
};

void delete_gui() {
    Fl::remove_timeout(callback);
    delete window;
}
