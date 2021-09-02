// Labarc riscv-isa-sim spike graphic user interface
// This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.1.
// Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2021

// This program is based in part on the work of the FLTK project (http://www.fltk.org)

#include "communicator.h"
#include <fstream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>

using std::string;
using std::vector;
using std::ifstream;
using std::find_if;

#define MAX_MEMS 10 // maximum active memory positions
#define NCHRS_DA_LINE 60 // number of characters in a dissassembly line

#define DISPLAY_FONT ((Fl_Font)55)
#define DISPLAY_FONT_SIZE 13
#define DISPLAY_FONT_WIDTH 8
#define DISPLAY_FONT_HEIGHT 18
#define NSTYLES 4 // number of styles used for disassembly
#define HELP_FONT ((Fl_Font)56)
#define HELP_FONT_SIZE 10
const char *mono_fonts[] = { "Noto Mono",
                             "Consolas",
                             "Monospace",
                             "Droid Sans Mono",
                             "Lucida Console",
			                    "DejaVu Sans Mono",
			                    "FreeMono", "" };
// define sub-windows geometries
#define BORDER 10
#define COMMAND_WIDTH 200
#define COMMAND_HEIGHT 20
#define PTEXT_OFFSET 2*BORDER+COMMAND_HEIGHT
#define PTEXT_LINES 5
#define DISA_OFFSET PTEXT_OFFSET+DISPLAY_FONT_HEIGHT*PTEXT_LINES+BORDER
#define DISA_LINES 6
#define REGS_OFFSET DISA_OFFSET+DISPLAY_FONT_HEIGHT*DISA_LINES+BORDER
#define REGS_LINES 9
#define MEMS_OFFSET REGS_OFFSET+DISPLAY_FONT_HEIGHT*REGS_LINES+BORDER
#define MEMS_LINES 5
#define WINDOW_HEIGHT MEMS_OFFSET+DISPLAY_FONT_HEIGHT*MEMS_LINES+BORDER
#define WINDOW_WIDTH 2*BORDER+((6+2+8)*4+2)*DISPLAY_FONT_WIDTH

class spike {
public:
   spike(int w, int h);
   communicator *sock;  // socket comunicator with spike
   void update(); // update register window
   static void cmd_cb(Fl_Widget *, void *); // static function called by callback
   Fl_Window window; // Window representing ISA simulator
private:
   Fl_Input command; // input debug command
   Fl_Text_Display ptext; //response display
   Fl_Text_Buffer *pbuff; // response text buffer - use pointer to avoid cb remove error
   Fl_Text_Display disa;  // disassembly display
   Fl_Text_Buffer *dbuff; // disassembly text buffer
   Fl_Text_Buffer *stylebuf;
   Fl_Text_Display::Style_Table_Entry
                   styletable[NSTYLES] = {     // Style table
                     { FL_BLACK,    DISPLAY_FONT, DISPLAY_FONT_SIZE+1 }, // A - Plain
                     { FL_DARK_RED, DISPLAY_FONT, DISPLAY_FONT_SIZE+1 }, // B - PC position
                     { FL_DARK3,    DISPLAY_FONT, DISPLAY_FONT_SIZE+1 }, // C - not PC position
                     { FL_BLACK,    DISPLAY_FONT, DISPLAY_FONT_SIZE+1 }  // D - do not use
                   };                                    // +1 makes font size equal do default
   Fl_Text_Display regs;   //register display
   Fl_Text_Buffer *rbuff; // register display text buffer 
   Fl_Text_Display mems;   // memory display
   Fl_Text_Buffer *mbuff; // memory display text buffer
   Fl_Window help_window; // help window
   Fl_Text_Display htext; // help text display
   Fl_Text_Buffer *hbuff; // help text buffer
   vector<string> disa_lines; // disassembled code, line by line  
   char style_str[(NCHRS_DA_LINE+1)*PTEXT_LINES];  // style buffer content
   string memcmds[MAX_MEMS]; // memory commands to be shown in memory window
   void cmd_caba(); // normal function called by callback function
   void disa_file(); // read disassembled code from file
   void help(); // show help window
};

spike::spike(int w, int h) :
    window(w, h, "RISC-V ISA simulator"),
    command(BORDER, BORDER, COMMAND_WIDTH, COMMAND_HEIGHT),
    ptext(BORDER,PTEXT_OFFSET,w-2*BORDER,DISPLAY_FONT_HEIGHT*PTEXT_LINES+1),
    disa(BORDER,DISA_OFFSET,w-2*BORDER,DISPLAY_FONT_HEIGHT*DISA_LINES+1),
    regs(BORDER,REGS_OFFSET,w-2*BORDER,DISPLAY_FONT_HEIGHT*REGS_LINES+1),
    mems(BORDER,MEMS_OFFSET,w-2*BORDER,DISPLAY_FONT_HEIGHT*MEMS_LINES+1),
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

    // Command
    command.align(FL_ALIGN_RIGHT);
    command.label("h for help");
    command.callback(cmd_cb, (void *)this);
    command.when(FL_WHEN_ENTER_KEY|FL_WHEN_NOT_CHANGED);

    // Response
    ptext.textfont(DISPLAY_FONT);
    pbuff = new Fl_Text_Buffer();
    pbuff->text("Command response");
    ptext.buffer(pbuff);

    // Disassmbly
    disa.textfont(DISPLAY_FONT);
    disa_file();
    dbuff = new Fl_Text_Buffer();
    stylebuf = new Fl_Text_Buffer();
    memset(style_str, 'C', (NCHRS_DA_LINE+1)*PTEXT_LINES-1);
    memset(style_str+NCHRS_DA_LINE+1, 'B', NCHRS_DA_LINE);    
    stylebuf->text(style_str);
    disa.buffer(dbuff);

    // Registers
    regs.textfont(DISPLAY_FONT);
    rbuff = new Fl_Text_Buffer();
    regs.buffer(rbuff);

    // Memory
    mems.textfont(DISPLAY_FONT);
    mbuff = new Fl_Text_Buffer();
    mems.buffer(mbuff);
    for(int i; i<MAX_MEMS; i++) memcmds[i] = "";

    window.end();
}


void spike::disa_file() { // read disassembly file into string vector
#define DISA_FILE "disa.txt"
  ifstream disa_file (DISA_FILE);
  if (disa_file.is_open()) {
    string li;
    // first line is an empty line so we can always show the line before
    li.append(NCHRS_DA_LINE, ' ');
    disa_lines.push_back(li);    
    while (getline (disa_file, li)) {
      li.append(NCHRS_DA_LINE - li.length(), ' ');
      disa_lines.push_back(li);
    }
    disa_file.close();
    li = "";  // append enough empty lines
    li.append(NCHRS_DA_LINE, ' ');
    for(int i=0; i<DISA_LINES; i++) disa_lines.push_back(li);    
  } else {
     cerr << "Unable to open file " DISA_FILE;
  }
}

void spike::update() {
   string pc = sock->send_and_rec("pc 0");

   // Disassembly
   string pcd = pc.substr(2,8) + ":"; // search for disassmbly line starting with pc
   vector<string>::iterator it = find_if(disa_lines.begin(),
                                         disa_lines.end(),
                                         [pcd](const std::string& str) {
                                            return str.substr(0,9) == pcd;
                                         }
                                        );
   if(it != disa_lines.end()) { // if found
      std::size_t i = std::distance(std::begin(disa_lines), it); // i>0 always
      string lis;
      for(int j=-1; j<DISA_LINES-2; j++) { // collect lines arround pc
        lis += disa_lines[i+j];
        if(j<DISA_LINES-3) lis += '\n';
      }
      dbuff->text(lis.c_str());
      disa.highlight_data(stylebuf, styletable, NSTYLES, 'D', NULL, NULL);
   } else dbuff->text("");

   // Registers
   string rg = sock->send_and_rec("reg 0");
   rg.erase( rg.end()-1 );
   rbuff->text(("  pc: " + pc + rg).c_str());

   // Memory
   string m;
   if (memcmds[0].c_str()[0]==0) m = "Memory - use mem command"; // hint if empty
   for (int i=0; memcmds[i].c_str()[0]; i++)  // for all slots which are not empty
      m += memcmds[i] + ": " + sock->send_and_rec(memcmds[i]);
   mbuff->text(m.c_str());
}

void spike::help() {
   hbuff = new Fl_Text_Buffer();
   hbuff->text(sock->send_and_rec(command.value()));
   fl_font(HELP_FONT, HELP_FONT_SIZE);
   htext.textfont(HELP_FONT);
   htext.buffer(hbuff);
   help_window.show();
}

void spike::cmd_cb(Fl_Widget *, void *ptr) {
   ((spike *)ptr)->cmd_caba();
}

void spike::cmd_caba() {
   if (command.value()[0] == 'h') {  // help
      help();
   } else if (command.value()[0] == 'q') {  // quit
      sock->send_and_rec(command.value());
      exit(0);
   } else if (strncmp(command.value(), "mem", 3)==0) {
      int i;
      for (i=0; memcmds[i].c_str()[0]; i++); // find first empty slot
      if (i==MAX_MEMS-1) {  // last position reached
         for (i=0; i<MAX_MEMS-1; i++) memcmds[i] = memcmds[i+1]; // shift up
         i = MAX_MEMS-2;
      }
      memcmds[i] = command.value();
      update();
   } else {
      pbuff->text(sock->send_and_rec(command.value()));
      update();
   }
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

    spike *spike_ptr = new spike(WINDOW_WIDTH, WINDOW_HEIGHT);
    spike_ptr->sock = new communicator(host, port);
    spike_ptr->update();
    spike_ptr->window.show(argc-argc_offset,argv+argc_offset);  // dirty argv[0] :-(

    Fl::run();   // run the graphical interface which calls cmd_cb()

    spike_ptr->sock->send_and_rec("q"); // when window is closed, send quit command to spike
    delete spike_ptr;

    // Fin
    exit(0);
}

