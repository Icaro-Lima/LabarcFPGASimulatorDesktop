// Labarc FPGA Simulator
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2019

#include "gui.h"

Fl_Window *window; // Window representing FPGA board
FPGA *fpga;

Fl_PNG_Image * FPGA::image = new Fl_PNG_Image(ASSET("FPGA.png"));
FPGA::FPGA(int x, int y) : Fl_Widget(x, y, image->w(), image->h()) {

       fl_font(DISPLAY_FONT, DISPLAY_FONT_SIZE);
       display_char_width = fl_width('8') + 0.7; // all characters are equal
       fl_font(LCD_FONT, LCD_FONT_SIZE);
       lcd_char_width = fl_width("8") + 0.7; // all hex digit characters are equal

	// Instance upper board of FPGA
	board = new Board(this->x() + image->w() / 2 - Board::image->w() / 2, this->y());
	
	// Instance LED's
	int led_offset = 30;
	int led_x = this->x() + image->w() / 2 - (LEDs::led_on->w() * 8 + 7 * (led_offset - LEDs::led_on->w())) / 2;
	leds = new LEDs(led_x, this->y() + LEDS_VERTICAL_OFFSET, led_offset);
	
        // system clock period chooser
        clk = new Clock(this->x() + image->w()-CLOCK_PERIOD_WIDTH-XMARGIN, this->y() + LEDS_VERTICAL_OFFSET);

	// Instance display
	disp = new display(this->x() + image->w() / 2, this->y() + image->h() / 2,
	                   (int)(LCD_NCHAR*lcd_char_width + 8*display_char_width + XMARGIN));
	disp->position(this->x() + image->w() / 2 - disp->w() / 2, this->y() + image->h() / 2 + 50 );
	  
        // Instance Hexadecimal Values
        hexv = new hexval(this->x() + image->w() / 2, this->y() + image->h() / 2,
                         (int)(LCD_NCHAR*lcd_char_width));
	hexv->position(this->x() + image->w() / 2 - hexv->w() / 2, this->y() + image->h() / 2 - 60 );

}

void FPGA::draw() {
	image->draw(x(), y());
	board->redraw();
	leds->redraw();
        clk->redraw();
	disp->redraw();
	hexv->redraw();
}

Fl_PNG_Image * Board::image = new Fl_PNG_Image(ASSET("Board.png"));
Board::Board(int x, int y) : Fl_Widget(x, y, image->w(), image->h()) {

	// Instance 7-segment display
	segments = new SegmentsDisplay(this->x() + image->w() / 2 - SegmentsDisplay::base->w() / 2,
	                               this->y() + image->h() / 2 - SegmentsDisplay::base->h() / 2);

	// Instance SWITCHES
	swis = new SWIs(this->x() + image->w() / 2 - (NSWIS * SWIs::swi_on->w() + (NSWIS-1) * (SWIS_OFFSET - SWIs::swi_on->w())
                                                   + SegmentsDisplay::base->w()) / 2,
	                this->y() + image->h() / 2 - SWIs::swi_on->h() / 2, SWIS_OFFSET);

}

Clock::Clock(int x, int y) : Fl_Spinner(x, y, CLOCK_PERIOD_WIDTH, 25, "FL_FLOAT_INPUT") {
        type(1);
        labelsize(16);
        minimum(0.5);
        maximum(3.0);
        step(0.5);
	value(0.5);
        textsize(18);
}

void Board::draw() {
	image->draw(x(), y());
	segments->redraw();
	swis->redraw();
}

SWI::SWI(int x, int y, int id) : 
	id(id), 
	state(false), 
	Fl_Widget(x, y, SWIs::swi_on->w(), SWIs::swi_on->h()) { }

void SWI::draw() {
	(state ? SWIs::swi_on : SWIs::swi_off)->draw(x(), y());
}

void SWIs::draw() {
	for (int i = 0; i < NSWIS; i++) { swis[i]->redraw(); }
}

Fl_PNG_Image * SWIs::swi_on = new Fl_PNG_Image(ASSET("SWIOn.png"));
Fl_PNG_Image * SWIs::swi_off = new Fl_PNG_Image(ASSET("SWIOff.png"));
SWIs::SWIs(int x, int y, int offset) : Fl_Widget(x, y, NSWIS * 33 + (NSWIS-1) * (offset - 33) + SegmentsDisplay::base->w(), 96) {
	for (int i = 0; i < NSWIS/2; i++) {
		swis[i] = new SWI(SegmentsDisplay::base->w() + x + offset * ((NSWIS-1) - i), y, i);
	}
	
	for (int i = NSWIS/2; i < NSWIS; i++) {
		swis[i] = new SWI(x + offset * ((NSWIS-1) - i), y, i);
	}
}

Fl_PNG_Image * LEDs::led_on = new Fl_PNG_Image(ASSET("LEDOn.png"));
Fl_PNG_Image * LEDs::led_off = new Fl_PNG_Image(ASSET("LEDOff.png"));
LEDs::LEDs(int x_origin, int y_origin, int offset) : Fl_Widget(x_origin, y_origin, 8 * led_on->w() + 7 * (offset - led_on->w()), led_on->h()) {
	this->x_origin = x_origin;
	this->y_origin = y_origin;
	this->offset = offset;
}

Fl_PNG_Image * SegmentsDisplay::base = new Fl_PNG_Image(ASSET("7SegmentsDisplayBase.png"));
SegmentsDisplay::SegmentsDisplay(int x, int y) : Fl_Widget(x, y, base->w(), base->h()) {
	this->previous = new bool[8];
	memset(this->previous, 0, 8 * sizeof(bool));
	
	point_on = new Fl_PNG_Image(ASSET("PointOn.png"));
	point_off = new Fl_PNG_Image(ASSET("PointOff.png"));
	vertical_on = new Fl_PNG_Image(ASSET("VerticalOn.png"));
	vertical_off = new Fl_PNG_Image(ASSET("VerticalOff.png"));
	horizontal_on = new Fl_PNG_Image(ASSET("HorizontalOn.png"));
	horizontal_off = new Fl_PNG_Image(ASSET("HorizontalOff.png"));
}

void SegmentsDisplay::draw_segments(char s) {
	(s>>0 & 1 ? horizontal_on : horizontal_off)->draw(x() + 26, y() + 25);
	(s>>1 & 1 ? vertical_on : vertical_off)->draw(x() + 74, y() + 33);
	(s>>2 & 1 ? vertical_on : vertical_off)->draw(x() + 74, y() + 88);
	(s>>3 & 1 ? horizontal_on : horizontal_off)->draw(x() + 26, y() + 135);
	(s>>4 & 1 ? vertical_on : vertical_off)->draw(x() + 17, y() + 88);
	(s>>5 & 1 ? vertical_on : vertical_off)->draw(x() + 17, y() + 33);
	(s>>6 & 1 ? horizontal_on : horizontal_off)->draw(x() + 26, y() + 80);
	(s>>7 & 1 ? point_on : point_off)->draw(x() + 91, y() + 134);
}

display::display(int x, int y, int w) : Fl_Widget(x, y, w, (3+1.5*NREG_LINES)*DISPLAY_FONT_SIZE + 2*LCD_FONT_SIZE + YMARGIN) { }

hexval::hexval(int x, int y, int w) : Fl_Widget(x, y, w, 2*LCD_FONT_SIZE+YMARGIN) { }

void display::lcd_labels() {
  fl_font(DISPLAY_FONT, DISPLAY_FONT_SIZE);
  fl_color(FL_RED);
  fl_draw("  pc       instruction     WriteData MemWrite", x() + XMARGIN, y() + DISPLAY_FONT_SIZE);
  fl_draw("Branch", x() + LCD_NCHAR*fpga->lcd_char_width, y() + 1.5*DISPLAY_FONT_SIZE + LCD_FONT_SIZE/2);
  fl_draw("SrcA  SrcB ALUResult Result ReadData MemtoReg", x() + XMARGIN, y() + 2*DISPLAY_FONT_SIZE + 2*LCD_FONT_SIZE + 2);
  fl_draw("RegWrite", x() + LCD_NCHAR*fpga->lcd_char_width, y() + 1.5*DISPLAY_FONT_SIZE + 1.5*LCD_FONT_SIZE);
  fl_color(FL_BLACK);
  fl_font(LCD_FONT, LCD_FONT_SIZE);
};

void display::register_labels() {
  int yy = y() + 3*DISPLAY_FONT_SIZE + 2*LCD_FONT_SIZE;
  int step = 1.5*DISPLAY_FONT_SIZE;
  fl_color(FL_RED);
  fl_font(DISPLAY_FONT, DISPLAY_FONT_SIZE);
  fl_draw("x0  zero      ra        sp        gp ", x() + XMARGIN, yy += step );
  fl_draw("x4  tp        t0        t1        t2 ", x() + XMARGIN, yy += step );
  fl_draw("x8  s0        s1        a0        a1 ", x() + XMARGIN, yy += step );
  fl_draw("x12 a2        a3        a4        a5 ", x() + XMARGIN, yy += step );
  fl_draw("x16 a6        a7        s2        s3 ", x() + XMARGIN, yy += step );
  fl_draw("x20 s4        s5        s6        s7 ", x() + XMARGIN, yy += step );
  fl_draw("x24 s8        s9        s10       s11", x() + XMARGIN, yy += step );
  fl_draw("x28 t3        t4        t5        t6 ", x() + XMARGIN, yy += step );
  fl_color(FL_BLACK);
}

void hexval::lcd_lines(long a, long b){
  fl_color(FL_BLACK);
  fl_font(LCD_FONT, LCD_FONT_SIZE);
  stringstream ss;
  ss << hex << setfill('0') << uppercase;
  // LCD data first line
  ss << setw(LCD_NCHAR) << a;
  fl_draw(ss.str().c_str(), x() + XMARGIN, y() + LCD_FONT_SIZE);
  // LCD data second line
  ss.str(""); // reset stringstream
  ss << setw(LCD_NCHAR) << b;
  fl_draw(ss.str().c_str(), x() + XMARGIN, y() + 2*LCD_FONT_SIZE);  
}

const char *mono_fonts[] = { "Noto Mono",
                             "Consolas",
                             "Monospace",
                             "Droid Sans Mono",
                             "Lucida Console",
			     "DejaVu Sans Mono",
			     "FreeMono", "" };

void init_gui(int argc, char** argv) {
	int window_width = FPGA::image->w();
	int window_height = FPGA::image->h();;
	window = new Fl_Window(Fl::w() / 2 - window_width / 2, Fl::h() / 2 - window_height / 2,
                               window_width, window_height, "Labarc FPGA Simulator");

	int i=0;
	do {  // search for an existing mono-space font
	  Fl::set_font(DISPLAY_FONT, mono_fonts[i++]);
	  fl_font(DISPLAY_FONT, DISPLAY_FONT_SIZE);
	} while( fl_width('W') != fl_width('i') && strlen(mono_fonts[i]) );

        Fl::set_font(LCD_FONT, "LED Counter 7");
        fl_font(LCD_FONT, LCD_FONT_SIZE);
        if( fl_width('1') != fl_width('8') || fl_width('1') != fl_width(' ') ) Fl::set_font(LCD_FONT, mono_fonts[i-1]);
			  
	// Instance FPGA
	fpga = new FPGA(0, 0);
	
	window->end();
	window->show(argc,argv);

	Fl::add_timeout(fpga->clk->value()/2, callback);       // set up first timeout for clock
};

void delete_gui() {
    Fl::remove_timeout(callback);
    delete fpga;
    delete window;
}

