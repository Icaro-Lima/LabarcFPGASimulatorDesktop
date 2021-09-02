// DESCRIPTION: Verilator: Systemverilog example module
// with interface to switch buttons, LEDs, LCD and register display

parameter divide_by=100000000;  // divisor do clock de referência
// A frequencia do clock de referencia é 50 MHz.
// A frequencia de clk_2 será de  50 MHz / divide_by

parameter NBITS_INSTR = 32;
parameter NBITS_TOP = 8, NREGS_TOP = 32, NBITS_LCD = 64;
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

  always_comb begin
    LED <= SWI;
    SEG <= SWI;
    lcd_WriteData <= SWI;
    lcd_pc <= 'h12;
    lcd_instruction <= 'h34567890;
    lcd_SrcA <= 'hab;
    lcd_SrcB <= 'hcd;
    lcd_ALUResult <= 'hef;
    lcd_Result <= 'h11;
    lcd_ReadData <= 'h33;
    lcd_MemWrite <= SWI[0];
    lcd_Branch <= SWI[1];
    lcd_MemtoReg <= SWI[2];
    lcd_RegWrite <= SWI[3];
    for(int i=0; i<NREGS_TOP; i++)
       if(i != NREGS_TOP/2-1) lcd_registrador[i] <= i+i*16;
       else                   lcd_registrador[i] <= ~SWI;
  end

  parameter NBITS=4;
  parameter NBITS_SEL=3;
  parameter NCOUNT=4;
  parameter INCR=3;
  logic [NBITS-1:0] a;
  // atribuição da entrada
  always_comb a <= SWI[7:4];

  logic reset;
  logic [NBITS-1:0] counter;
  // atribuição da entrada de reset
  always_comb reset <= SWI[0];
  // entrada do inicio da contagem
  always_comb a <= SWI[7:4];

  always_ff @(posedge clk_2)
     if (reset) begin
        counter <= a;
     end else begin
        counter <= counter + INCR;
     end

  logic [NBITS-1:0] va [1:NCOUNT];
  logic [NBITS_SEL-1:0] seletor;
  // atribuição da entrada
  always_comb seletor <=SWI[3:1];

  always_comb begin
     va[1] <= a  + 3;
     for(int i=1; i<NCOUNT; i++) va[i+1] <= va[i] + INCR;
  end

  // atribuição da saída
  always_comb begin
     lcd_a <= a;
     if(seletor>0 && NCOUNT>=seletor) lcd_b <= va[seletor];
     else if(seletor==0)              lcd_b <= counter;
     else                             lcd_b <= 0;
   end

endmodule
