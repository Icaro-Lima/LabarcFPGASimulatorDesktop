// Labarc FPGA Simulator
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2019

#include <sstream>
#include <iomanip>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Spinner.H>
#include <FL/Fl_Window.H>

using std::stringstream;
using std::hex;
using std::setw;
using std::setfill;
using std::uppercase;
using std::nouppercase;
using std::string;

#define ASSETS_DIR "Assets/"
#define ASSET(F) (string(ASSETS_DIR) + F).c_str()
#define XMARGIN 10
#define YMARGIN 10
#define NSWIS 8
#define SWIS_OFFSET 64
#define NLEDS 8
#define LEDS_VERTICAL_OFFSET 230
#define CLOCK_PERIOD_WIDTH 50
#define LCD_FONT ((Fl_Font)56)
#define LCD_FONT_SIZE 32
#define LCD_NCHAR 16
#define DISPLAY_FONT ((Fl_Font)55)
#define DISPLAY_FONT_SIZE 13
#define NREGS 32
#define NREGS_PER_LINE 4
#define NREG_LINES (NREGS/NREGS_PER_LINE)
#define CLOCK 0.25

class SWI : public Fl_Widget {
	int id;
public:
	bool state;
	
	virtual void draw();
	virtual int handle(int event);
	SWI(int x, int y, int id);
};

class SWIs : public Fl_Widget {
	SWI *swis[NSWIS];
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
  display(int x, int y, int w);
private:
  int offset;
  void lcd_labels();
  void register_labels();
};

class hexval : public Fl_Widget {
public:
  void draw();
  hexval(int x, int y, int w);
private:
  int offset;
  void lcd_lines(long a, long b);
};

class SegmentsDisplay : public Fl_Widget {
	bool *previous;
	Fl_PNG_Image *point_on, *point_off, *vertical_on, *vertical_off, *horizontal_on, *horizontal_off;
	
public:
	static Fl_PNG_Image *base;

	virtual void draw();
	SegmentsDisplay(int x, int y);
private:
        void draw_segments(char s);
};

class Board : public Fl_Widget {		
public:
	static Fl_PNG_Image *image;

	virtual void draw();
	Board(int x, int y);

        SWIs *swis;  // switches
        SegmentsDisplay *segments; // seven LED segment display 
};

class Clock : public Fl_Spinner {
public:
	Clock(int x, int y);
};

class FPGA : public Fl_Widget {	
public:
	static Fl_PNG_Image *image;
        float display_char_width;
        float lcd_char_width;
        Clock *clk;

	FPGA(int x, int y);
	virtual void draw();
private:
	Board *board;  // printed circuit board attached to the FPGA board
        LEDs *leds;
        display *disp; // RISC-V processor display
	hexval *hexv; // display for two hexadecimal number of 16 digits each
};

void init_gui(int, char**);
void delete_gui();
void callback(void*);

extern FPGA *fpga;


