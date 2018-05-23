// DESCRIPTION: Verilator: Verilog example module
/* verilator lint_off COMBDLY */
/* verilator lint_off WIDTH */

// numero de bits da instrucao
parameter NINSTR_BITS = 32;
// Este parametro e global porque nao faz sentido mudar-lo,
// ja que queremos preservar a compatibilidade do processador com a toolchain RISC-V. 

// Interface para espiar para dentro do processador
interface zoi #(parameter NBITS = 8, NREGS = 32) ();
  logic [NBITS-1:0] pc;
  logic [NINSTR_BITS-1:0] instruction;
  logic [NBITS-1:0] SrcA, SrcB;
  logic [NBITS-1:0] ALUResult, Result;
  logic [NBITS-1:0] registrador [0:NREGS-1];
  logic [NBITS-1:0] WriteData, ReadData;
  logic MemWrite, Branch, MemtoReg, RegWrite;
endinterface

parameter NBITS_TOP = 8, NREGS_TOP = 32;
module top(input  logic clk_2,
           input  logic [NBITS_TOP-1:0] SWI,
           output logic [NBITS_TOP-1:0] LED,
           output logic [NINSTR_BITS-1:0] lcd_instruction,
           output logic [NBITS-1:0] lcd_registrador [0:NREGS-1],
           output logic [NBITS_TOP-1:0] lcd_pc);

logic clock, reset;
always_ff @(posedge clk_2) if(SWI[7]) clock <= ~clock;
always_comb begin
  LED[7] <= clock;
  reset <= SWI[6];
end

// a CPU
localparam NBITS = 8;   // numero de bits dos barramentos de enderecos e dados
localparam NREGS = 32;  // numero de registradores no banco de registradores
zoi #(.NBITS(NBITS), .NREGS(NREGS)) z();

// dispositivos de entrada e saida
localparam NIO_BITS = 5; // nao tem nem SWI nem LED suficiente para ter NIO_BITS=NBITS
logic [NBITS-1:0] entrada, saida;
always_comb begin
  entrada[NIO_BITS-1:0] <= SWI[NIO_BITS-1:0];
  LED[NIO_BITS-1:0] <= saida[NIO_BITS-1:0];
end

// zoiada para LCD
always_comb begin
   lcd_pc <= z.pc;
   lcd_instruction <= z.instruction;
   lcd_registrador <= z.registrador;
end

endmodule
