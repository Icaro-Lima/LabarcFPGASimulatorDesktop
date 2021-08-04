// Labarc riscv-isa-sim spike graphic user interface
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2021

// This program is based in part on the work of the FLTK project (http://www.fltk.org) and

#include "communicator.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>

#define BORDER 10

#define DISPLAY_FONT ((Fl_Font)55)
#define DISPLAY_FONT_SIZE 13
#define DISPLAY_FONT_WIDTH 8
#define DISPLAY_FONT_HEIGHT 18
#define HELP_FONT ((Fl_Font)56)
#define HELP_FONT_SIZE 10
const char *mono_fonts[] = { "Noto Mono",
                             "Consolas",
                             "Monospace",
                             "Droid Sans Mono",
                             "Lucida Console",
			                    "DejaVu Sans Mono",
			                    "FreeMono", "" };

class spike {
public:
   Fl_Window window; // Window representing ISA simulator
   Fl_Input command; // input debug command
   Fl_Text_Display ptext; //response display
   Fl_Text_Buffer *pbuff; // response text buffer - use pointer to avoid cb remove error
   Fl_Text_Display regs;   //register display
   Fl_Text_Buffer *rbuff; // register display text buffer 
   Fl_Text_Display mems;   // memory display
   Fl_Text_Buffer *mbuff; // memory display text buffer
   Fl_Window help_window; // help window
   Fl_Text_Display htext; // help text display
   Fl_Text_Buffer *hbuff; // help text buffer
   communicator *sock;  // socket comunicator with spike
   spike(int w, int h);
   void open(char *host, char *port);  // open communicator socket
   void update(); // update register window
   void help();
};

spike *spike_ptr;

void spike::update() {
   string pc = sock->send_and_rec("pc 0");
   string rg = sock->send_and_rec("reg 0");
   rg.erase( rg.end()-1 );
   rbuff->text(("  pc: " + pc + rg).c_str());
}

void spike::help() {
   hbuff = new Fl_Text_Buffer();
   hbuff->text(sock->send_and_rec(command.value()));
   fl_font(HELP_FONT, HELP_FONT_SIZE);
   htext.textfont(HELP_FONT);
   htext.buffer(hbuff);
   help_window.show();
}

void cmd_cb(Fl_Widget *, void* v) {
   if (spike_ptr->command.value()[0] == 'h') {  // help
      spike_ptr->help();
   } else if (spike_ptr->command.value()[0] == 'q') {  // quit
      spike_ptr->sock->send_and_rec(spike_ptr->command.value());
      delete spike_ptr;
      exit(0);
   } else {
      spike_ptr->pbuff->text(spike_ptr->sock->send_and_rec(spike_ptr->command.value()));
      spike_ptr->update();
   }
}

#define COMMAND_WIDTH 200
#define COMMAND_HEIGHT 20
#define PTEXT_OFFSET 2*BORDER+COMMAND_HEIGHT
#define PTEXT_LINES 5
#define REGS_OFFSET PTEXT_OFFSET+DISPLAY_FONT_HEIGHT*PTEXT_LINES+BORDER
#define REGS_LINES 9
#define MEMS_OFFSET REGS_OFFSET+DISPLAY_FONT_HEIGHT*REGS_LINES+BORDER
#define MEMS_LINES 5
#define WINDOW_HEIGHT MEMS_OFFSET+DISPLAY_FONT_HEIGHT*MEMS_LINES+BORDER
#define WINDOW_WIDTH 2*BORDER+((6+2+8)*4+2)*DISPLAY_FONT_WIDTH
#define HELP_LINES (PTEXT_LINES+REGS_LINES+MEMS_LINES)

spike::spike(int w, int h) :
    window(w, h, "RISC-V ISA simulator"),
    command(BORDER, BORDER, COMMAND_WIDTH, COMMAND_HEIGHT),
    ptext(BORDER,PTEXT_OFFSET,w-2*BORDER,DISPLAY_FONT_HEIGHT*PTEXT_LINES),
    regs(BORDER,REGS_OFFSET,w-2*BORDER,DISPLAY_FONT_HEIGHT*REGS_LINES),
    mems(BORDER,MEMS_OFFSET,w-2*BORDER,DISPLAY_FONT_HEIGHT*MEMS_LINES),
    help_window(w,h, "Spike Help"),
    htext(BORDER,BORDER,w-2*BORDER,h-2*BORDER) {
    int i=0;
    do {  // search for an existing mono-space font
      Fl::set_font(DISPLAY_FONT, mono_fonts[i++]);
      fl_font(DISPLAY_FONT, DISPLAY_FONT_SIZE);
    } while( fl_width('W') != fl_width('i') && strlen(mono_fonts[i]) );
    // set help font to a different font if there is another one
    if (strlen(mono_fonts[i])) Fl::set_font(HELP_FONT, mono_fonts[i]);
    else                       Fl::set_font(HELP_FONT, mono_fonts[i-1]);
    ptext.textfont(DISPLAY_FONT);
    regs.textfont(DISPLAY_FONT);
    mems.textfont(DISPLAY_FONT);
    pbuff = new Fl_Text_Buffer();
    pbuff->text("Command output");
    ptext.buffer(pbuff);
    rbuff = new Fl_Text_Buffer();
    regs.buffer(rbuff);
    mbuff = new Fl_Text_Buffer();
    mbuff->text("Memory - use mem command");
    mems.buffer(mbuff);
    command.align(FL_ALIGN_RIGHT);
    command.label("h for help");
    command.callback(cmd_cb, &window);
    command.when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);
    window.end();
}   

void spike::open(char *host, char *port) {
   sock = new communicator(host, port);
}

int main(int argc, char** argv, char** env) {

    if (argc < 2)
    {
      std::cerr << "Usage: " << argv[0] << " [<host>] <port>\n";
      return 1;
    }

    char *host;
    char *port;
    int argc_offset;
    if(argc==2) {  // default for host is localhost
       host = (char *)"localhost";
       port = argv[1];
       argc_offset = 1;
    } else {
       host = argv[1];
       port = argv[2];
       argc_offset = 2;
    }

    spike_ptr = new spike(WINDOW_WIDTH, WINDOW_HEIGHT);
    spike_ptr->open(host, port);
    spike_ptr->update();
    spike_ptr->window.show(argc-argc_offset,argv+argc_offset);  // dirty argv[0] :-(

    Fl::run();   // run the graphical interface which calls cmd_cb()

    spike_ptr->sock->send_and_rec("q"); // when window is closed, send quit command to spike
    delete spike_ptr;

    // Fin
    exit(0);
}

