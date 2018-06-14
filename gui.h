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
#include <FL/Fl_Window.H>

using std::stringstream;
using std::hex;
using std::setw;
using std::setfill;
using std::uppercase;
using std::nouppercase;
using std::string;

#define XMARGIN 10
#define SWIS_OFFSET 64
#define LEDS_VERTICAL_OFFSET 230
#define DISPLAY_FONT ((Fl_Font)55)
#define ASSETS_DIR "Assets/"
#define ASSET(F) (string(ASSETS_DIR) + F).c_str()

class SWI : public Fl_Widget {
	int id;
public:
	bool state;
	
	virtual void draw();
	virtual int handle(int event);
	SWI(int x, int y, int id);
};

class SWIs : public Fl_Widget {
	SWI *swis[8];
public:
	static Fl_PNG_Image *swi_on, *swi_off;

	SWIs(int x, int y, int offset);
	virtual void draw();
};

class LEDs : public Fl_Widget {
	int x_origin, y_origin, offset;	
public:	
	static Fl_PNG_Image *led_on, *led_off;

	LEDs(int x_origin, int y_origin, int offset);
	virtual void draw();
};

// show SystemVerilog output signal in graphic interface
class display : public Fl_Widget {  // FLTK Widget
public:
  void draw();
  void lcd_labels(int start, int step);
  void register_labels(int start, int step);
  display(int x, int y);
private:
  int offset;
};

class SegmentsDisplay : public Fl_Widget {
	bool *previous;
	Fl_PNG_Image *point_on, *point_off, *vertical_on, *vertical_off, *horizontal_on, *horizontal_off;
	
public:
	static Fl_PNG_Image *base;

	virtual void draw();
	SegmentsDisplay(int x, int y);
};

class Board : public Fl_Widget {		
public:
	static Fl_PNG_Image *image;

	virtual void draw();
	Board(int x, int y);
};

class FPGA : public Fl_Widget {	
public:
	static Fl_PNG_Image *image;

	virtual void draw();
	FPGA(int x, int y);
};

extern Fl_Window *window;
extern FPGA *fpga;
extern LEDs *leds;
extern display *disp;
extern Board *board;
extern SWIs *swis;
extern SegmentsDisplay *segments;

void init_gui(int, char**);
void delete_gui();
void callback(void*);

