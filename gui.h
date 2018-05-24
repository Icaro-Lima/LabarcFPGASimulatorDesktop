// Labarc FPGA Simulator
// This program is based in part on the work of the FLTK project (http://www.fltk.org).
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2018

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>

#define XMARGIN 10

// set SystemVerilog input signal from graphic interface
#define NBUTTONS 8
class SWI_Buttons {
    Fl_Button *b[NBUTTONS];
    static void toggle_cb(Fl_Widget *o, SWI_Buttons* this_o); 
    char label[NBUTTONS][2];
  public:
    SWI_Buttons(int x, int y, int offset, int width, int height);
};

#define STR_LENGTH 50
// show SystemVerilog output signal in graphic interface
class display : public Fl_Widget {  // FLTK Widget
  char str[STR_LENGTH];
public:
  void draw();
  void lcd_labels(int start, int step);
  void register_labels(int start, int step);
  display(int x,int y, int offset, int width, int height);
  display(Fl_Window &window, int y, int offset, int width, int height);
private:
  int offset;
};

extern Fl_Window *window;
extern SWI_Buttons *swi;
extern display *disp;

void init_gui(int, char**);
void delete_gui();
void callback(void*);

