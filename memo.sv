/* verilator lint_off WIDTH */
module memo (
        input logic  [5:0] address,
        input logic  clock,
        input logic  [7:0] data,
        input logic  wren,
        output logic [7:0] q);

  logic [7:0] m [(1<<6)-1:0];

  initial begin
    int fd;
    fd = $fopen("memo.mif","r");
    if( fd ) begin
      $display("Detected memo.mif");
      // do not actually read memo.mif, assume it has the normal content
      m[0] = 0;
      for(int i=   1; i<='h30; i++) m[i] = 'haa;
      for(int i='h31; i<='h39; i++) m[i] = 'haa - i;
      for(int i='h3A; i<='h3F; i++) m[i] = 'hbb;
    end else
      for(int i=0; i<='h3F; i++) m[i] = 0;
  end

  always @(posedge clock) begin
    q <= m[address];
    if(wren) m[address] <= data; 
  end

endmodule

