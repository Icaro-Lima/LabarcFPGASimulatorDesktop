module divi (input logic clock,
             input logic [31:0]  denom,
             input logic [62:0]  numer,
             output logic [62:0]  quotient);

parameter NPIPE=16;

logic [62:0] q[0:NPIPE-1];

always_ff @(posedge clock) begin
   for(int i=0; i<NPIPE-1; i++) q[i+1] <= q[i];
   q[0] <= numer/denom;
end
 
always_comb quotient <= q[NPIPE-1];

endmodule


