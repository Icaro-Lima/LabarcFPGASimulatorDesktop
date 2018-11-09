// DESCRIPTION: Verilator: Systemverilog example module
// with interface to switch buttons, LEDs, LCD and register display

parameter NBITS_TOP = 8;
module top(input  logic clk_2,
           input  logic [NBITS_TOP-1:0] SWI,
           output logic [NBITS_TOP-1:0] LED,
           output logic [NBITS_TOP-1:0] SEG,
           output logic [64-1:0] LCD);

  always_comb begin
    LED <= SWI;
    SEG <= SWI;
    LCD <= count;
  end

logic [63:0] count;

always_ff @ (posedge clk_2) begin
    count++;
end
  

endmodule
