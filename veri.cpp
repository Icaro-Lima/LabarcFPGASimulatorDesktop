#include <iostream>
#include <iomanip>
#include <string>

using std::string;
using std::ostream;
using std::setw;
using std::endl;

// Include common Verilator routines
#include <verilated.h>

// Include Verilator model header, generated from Verilating "top.v"
#include "Vtop.h"
#define NREGS 16

Vtop* top;   // Verilated model

vluint64_t main_time = 0;       // Current Verilator simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  You can also use a double, if you wish.
// One main_time unit corresponds to 0.1 seconds in real time

double sc_time_stamp () {       // Called by $time in Verilog
     return main_time;          // converts to double, to match what SystemC does
}

void tick_v() {
    main_time++;            // Verilator simulation time passes...

    top->clk_2 = !top->clk_2;       // Toggle clock

    top->eval();  // Evaluate Verilated SystemVerilog model
}

void swi(unsigned short cmd) {
    unsigned short p = 1<<((cmd & 0xE)>>1); // bit position
    if( cmd&1 ) top->SWI |= p;  // set bit at position
    else        top->SWI &= ~p; // clear bit at position
    top->eval();  // Evaluate Verilated SystemVerilog model
}

// macro for SystemVerilog top module output
#define s(top_port)  setw(2) << (unsigned short)top->top_port

void riscv(unsigned short cmd, ostream& sout) {
//  $cmd    # of bytes
//          returned       description
// -------------------------------------------------------------------
// 0000xxxx     16      RISC-V registers 0 to 15
// 00100011     12      RISC-V pc, instruction, SrcA, SrcB, ..., flags
// 0011xxxx     16      LCD 1st and 2nd line
// all others    2      LED and SEG
	 if ( (cmd & 0xF0) == 0x00) {  // cmd = 0000xxxx - RISC-V registers
            for (int i=0; i<NREGS; i++) sout << s(lcd_registrador[i]);
         } else if ( (cmd & 0xF0) == 0x30) {  // cmd = 0011xxxx - LCD
            sout << setw(16) << (unsigned long)top->lcd_b
                 << setw(16) << (unsigned long)top->lcd_a;
         } else if (cmd == 0x23) { // cmd = 001000011 - RISC-V pc, ...
            sout << s(lcd_pc) << setw(8) << (unsigned int)top->lcd_instruction
                 << s(lcd_SrcA) << s(lcd_SrcB) << s(lcd_ALUResult) << s(lcd_Result)
                 << s(lcd_WriteData) << s(lcd_ReadData)
                 << setw(2) << ( (top->lcd_RegWrite <<3) | (top->lcd_MemtoReg <<2) 
                               | (top->lcd_Branch   <<1) |  top->lcd_MemWrite     ); 
         } else { // all other cmd
            sout << s(SEG) << s(LED);
         }
}

void vinit(int argc, char** argv) {
    // Construct the Verilated model, from Vtop.h generated from Verilating "top.v"
    top = new Vtop;

    top->SWI = 0;     // Initial settings of inputs
    top->clk_2 = 0;

    Verilated::commandArgs(argc, argv);   // Remember args
}

extern void vdelete() {
    delete top;
}

