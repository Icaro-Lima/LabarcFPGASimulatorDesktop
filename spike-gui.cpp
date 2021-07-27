// Labarc riscv-isa-sim spike graphic user interface
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2021

// This program is based in part on the work of the FLTK project (http://www.fltk.org) and
// https://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp03/echo/blocking_tcp_echo_client.cpp

#include "communicator.h"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>

#define DISPLAY_FONT ((Fl_Font)55)
#define DISPLAY_FONT_SIZE 13
#define DISPLAY_FONT_WIDTH 8
#define DISPLAY_FONT_HEIGHT 18
#define BORDER 10
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
   communicator *sock;
   spike(int w, int h);
   void open(char *host, char *port);  // open communicator sock
};

spike *spike_ptr;

void cmd_cb(Fl_Widget *, void* v) {
  spike_ptr->pbuff->text(spike_ptr->sock->send_and_rec(spike_ptr->command.value()));
  string pc = spike_ptr->sock->send_and_rec("pc 0");
  string rg = spike_ptr->sock->send_and_rec("reg 0");
  rg.erase( rg.end()-1 );
  spike_ptr->rbuff->text(("  pc: " + pc + rg).c_str());
}

spike::spike(int w, int h) :
    window(w, h, "RISC-V ISA simulator"),
    command(BORDER, BORDER, 200, 20),
    ptext(BORDER,50,w-2*BORDER,200),
    regs(BORDER,250,w-2*BORDER,DISPLAY_FONT_HEIGHT*9),
    mems(BORDER,450,w-2*BORDER,h-BORDER-450) {
    int i=0;
    do {  // search for an existing mono-space font
      Fl::set_font(DISPLAY_FONT, mono_fonts[i++]);
      fl_font(DISPLAY_FONT, DISPLAY_FONT_SIZE);
    } while( fl_width('W') != fl_width('i') && strlen(mono_fonts[i]) );
    ptext.textfont(DISPLAY_FONT);
    regs.textfont(DISPLAY_FONT);
    mems.textfont(DISPLAY_FONT);
    pbuff = new Fl_Text_Buffer();
    pbuff->text("Oip");
    ptext.buffer(pbuff);
    rbuff = new Fl_Text_Buffer();
    rbuff->text("Oir");
    regs.buffer(rbuff);
    mbuff = new Fl_Text_Buffer();
    mbuff->text("Oim");
    mems.buffer(mbuff);
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

    spike_ptr = new spike(2*BORDER+((6+2+8)*4+2)*DISPLAY_FONT_WIDTH, 600);
    spike_ptr->open(host, port);
    spike_ptr->window.show(argc-argc_offset,argv+argc_offset);  // dirty argv[0] :-(

    Fl::run();   // run the graphical interface which calls cmd_cb()

    spike_ptr->sock->send_and_rec("q"); // when window is closed, send quit command to spike
    delete spike_ptr;

    // Fin
    exit(0);
}

