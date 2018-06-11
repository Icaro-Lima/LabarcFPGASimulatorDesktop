// Labarc FPGA Simulator
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2018

#include <sstream>
#include <iomanip>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Double_Window.H>

using namespace std;

#define XMARGIN 10
#define DISPLAY_FONT ((Fl_Font)55)

class SWI : public Fl_Widget {
	int x, y, id;
	Fl_PNG_Image *swi_on, *swi_off;
public:
	bool state;
	
	virtual int handle(int event);
	virtual void draw();
	SWI(int x, int y, int id, Fl_PNG_Image *swi_on, Fl_PNG_Image *swi_off);
};

class SWIs {
	SWI *swis[8];
	Fl_PNG_Image *swi_on, *swi_off;
public:
	void draw();
	void toggle_cb();
	SWIs(int x, int y, int offset);
};

class LEDs {
	int x_origin, y_origin, offset;
	Fl_PNG_Image *led_on, *led_off;
	
public:
	bool *states;
	
	LEDs(int x_origin, int y_origin, int offset);
	void draw();
};

// show SystemVerilog output signal in graphic interface
class display : public Fl_Widget {  // FLTK Widget
public:
  void draw();
  void lcd_labels(int start, int step);
  void register_labels(int start, int step);
  display(int x, int y, int width, int height);
private:
  int offset;
};

class SegmentsDisplay {
	int x_origin, y_origin;
	bool first_draw;
	bool *previous;
	Fl_PNG_Image *base, *point_on, *point_off, *vertical_on, *vertical_off, *horizontal_on, *horizontal_off;
	
public:
	void draw();
	SegmentsDisplay(int x_origin, int y_origin);
};

extern Fl_Window *window;
extern SWIs *swi;
extern LEDs *leds;
extern display *disp;
extern SegmentsDisplay *segments;

void init_gui(int, char**);
void delete_gui();
void callback(void*);

