// numero de bits da instrucao
parameter NBITS_INSTR = 32;
// Este parametro e global porque nao faz sentido mudar-lo,
// ja que queremos preservar a compatibilidade do processador com a toolchain RISC-V. 
parameter NBITS_TOP = 8, NREGS_TOP = 32, NBITS_LCD = 64;

parameter divide_by=100000000;  // divisor do clock de referência
// A frequencia do clock de referencia é 50 MHz.
// A frequencia de clk_2 será de  50 MHz / divide_by

// Interface para espiar para dentro do processador
interface zoi #(parameter NBITS = 8, NREGS = 32) ();
  logic [NBITS-1:0] pc;
  logic [NBITS_INSTR-1:0] instruction;
  logic [NBITS-1:0] SrcA, SrcB;
  logic [NBITS-1:0] ALUResult, Result;
  logic [NBITS-1:0] registrador [0:NREGS-1];
  logic [NBITS-1:0] WriteData, ReadData;
  logic MemWrite, Branch, MemtoReg, RegWrite;
endinterface

`include "cpu.sv"

module top(input  logic clk_2,
           input  logic [NBITS_TOP-1:0] SWI,
           output logic [NBITS_TOP-1:0] LED,
           output logic [NBITS_TOP-1:0] SEG,
           output logic [NBITS_LCD-1:0] lcd_a, lcd_b,
           output logic [NBITS_INSTR-1:0] lcd_instruction,
           output logic [NBITS_TOP-1:0] lcd_registrador [0:NREGS_TOP-1],
           output logic [NBITS_TOP-1:0] lcd_pc, lcd_SrcA, lcd_SrcB,
             lcd_ALUResult, lcd_Result, lcd_WriteData, lcd_ReadData, 
           output logic lcd_MemWrite, lcd_Branch, lcd_MemtoReg, lcd_RegWrite);

// clock lento //
logic pause; // parar clock
logic clock; // clock de sistema lento 

always_comb begin
  pause <= SWI[7];
  SEG[7] <= clock;
end

always_ff @(posedge clk_2)
  if (~pause) clock <= ~clock;

// reset //
logic reset;
always_comb reset <= SWI[6];


// a CPU
zoi #(.NBITS(NBITS_TOP), .NREGS(NREGS_TOP)) z();
cpu #(.NBITS(NBITS_TOP), .NREGS(NREGS_TOP)) c(.*);

// dispositivos de entrada e saida
localparam NIO_BITS = 5; // nao tem nem SWI nem LED suficiente para ter NIO_BITS=NBITS
logic [NBITS_TOP-1:0] entrada, saida;
always_comb begin
  entrada[NIO_BITS-1:0] <= SWI[NIO_BITS-1:0];
  LED[NIO_BITS-1:0] <= saida[NIO_BITS-1:0];
end

// zoiada para LCD
always_comb begin
   lcd_pc <= z.pc;
   lcd_instruction <= z.instruction;
   lcd_SrcA <= z.SrcA;
   lcd_SrcB <= z.SrcB;
   lcd_ALUResult <= z.ALUResult;
   lcd_Result <= z.Result;
   lcd_WriteData <= z.WriteData;
   lcd_ReadData <= z.ReadData;
   lcd_MemWrite <= z.MemWrite;
   lcd_Branch <= z.Branch;
   lcd_MemtoReg <= z.MemtoReg;
   lcd_RegWrite <= z.RegWrite;
   lcd_registrador <= z.registrador;
end

endmodule
