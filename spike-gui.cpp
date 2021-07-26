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
   communicator *sock;
   spike(int w, int h);
   void open(char *host, char *port);  // open communicator sock
};

spike *spike_ptr;

void cmd_cb(Fl_Widget *, void* v) {
  spike_ptr->pbuff->text(spike_ptr->sock->send_and_rec(spike_ptr->command.value()));
}

spike::spike(int w, int h) :
    window(w, h, "RISC-V ISA simulator"),
    command(BORDER, BORDER, 200, 20),
    ptext(BORDER,50,w-2*BORDER,h-50-BORDER) {
    int i=0;
    do {  // search for an existing mono-space font
      Fl::set_font(DISPLAY_FONT, mono_fonts[i++]);
      fl_font(DISPLAY_FONT, DISPLAY_FONT_SIZE);
    } while( fl_width('W') != fl_width('i') && strlen(mono_fonts[i]) );   
    ptext.textfont(DISPLAY_FONT);
    command.callback(cmd_cb, &window);
    pbuff = new Fl_Text_Buffer();
    pbuff->text("Oi");
    ptext.buffer(pbuff);
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

    spike_ptr = new spike(600, 300);
    spike_ptr->open(host, port);
    spike_ptr->window.show(argc-argc_offset,argv+argc_offset);  // dirty argv[0] :-(

    Fl::run();   // run the graphical interface which calls cmd_cb()

    spike_ptr->sock->send_and_rec("q"); // when window is closed, send quit command to spike
    delete spike_ptr;

    // Fin
    exit(0);
}

