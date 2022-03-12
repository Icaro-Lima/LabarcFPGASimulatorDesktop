module lcd_64bit (input logic clk, reset,
            input logic [63:0] a, b,
            output logic LCD_RS, LCD_E,
            output logic [7:0] LCD_D);

`define NBITS_TIME 5
logic [`NBITS_TIME-1:0] waitfor; // log2 of delay value
logic [(1<<`NBITS_TIME)-1:0] delay; // in units of 20 ns
`define NS /20
`define US *50
`define MS *50000

logic [5:0] idx; // table index

enum logic [2:0] { getidx, setup, enable, hold } wstate;
always_ff @(posedge clk)
   if (reset) begin
      idx <= 0;
      LCD_E <= 0;
      delay <= 0;
      wstate <= getidx;
   end else
      unique case (wstate)
         getidx: begin
                    wstate <= setup;
                    delay <= 1<<waitfor; // 2^waitfor
                 end
         setup:  if(delay>0) delay <= delay-1;
                 else begin
                    LCD_E <= 1;
                    wstate <= enable;
                    delay <= 500`NS;
                 end
         enable: if(delay>0) delay <= delay-1;
                 else begin
                    LCD_E <= 0;
                    wstate <= hold;
                    delay <= 500`NS;
                 end
         hold:   if(delay>0) delay <= delay-1;
                 else begin
                    if(idx<idx_max) idx <= idx + 1;
                    else idx <= idx_loop;
                    wstate <= getidx;
                 end
      endcase

function [8:0] hex2char;
	input [3:0] h;
	hex2char = (h>9 ? 9'h137 : 9'h130) + h;	
endfunction

`define SEND_LCD(delay,data) { waitfor, LCD_RS, LCD_D } <= { `NBITS_TIME'($clog2( delay )), data }

always_comb case (idx)  // delay values are stored as their corresponding log2 values
       // http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
       0: `SEND_LCD(150`MS, 9'h030 ); // Special case of Function Set
       1: `SEND_LCD(  5`MS, 9'h030 ); // Special case of Function Set
       2: `SEND_LCD(120`US, 9'h030 ); // Special case of Function Set
       3: `SEND_LCD(120`US, 9'h038 ); // Function Set  2 lines 
       4: `SEND_LCD( 55`US, 9'h008 ); // Display off
       5: `SEND_LCD( 55`US, 9'h001 ); // clear Display
       6: `SEND_LCD(  3`MS, 9'h006 ); // Entry mode Increment, no shift
       7: `SEND_LCD( 55`US, 9'h00C ); // Display on

       8: `SEND_LCD( 55`US, 9'h080 ); // DDRAM address=07
       9: `SEND_LCD( 55`US, hex2char(a[63:60]) );
      10: `SEND_LCD( 55`US, hex2char(a[59:56]) );
      11: `SEND_LCD( 55`US, hex2char(a[55:52]) );
      12: `SEND_LCD( 55`US, hex2char(a[51:48]) );
      13: `SEND_LCD( 55`US, hex2char(a[47:44]) );
      14: `SEND_LCD( 55`US, hex2char(a[43:40]) );
      15: `SEND_LCD( 55`US, hex2char(a[39:36]) );
      16: `SEND_LCD( 55`US, hex2char(a[35:32]) );
      17: `SEND_LCD( 55`US, hex2char(a[31:28]) ); 
      18: `SEND_LCD( 55`US, hex2char(a[27:24]) ); 
      19: `SEND_LCD( 55`US, hex2char(a[23:20]) ); 
      20: `SEND_LCD( 55`US, hex2char(a[19:16]) ); 
      21: `SEND_LCD( 55`US, hex2char(a[15:12]) ); 
      22: `SEND_LCD( 55`US, hex2char(a[11: 8]) ); 
      23: `SEND_LCD( 55`US, hex2char(a[ 7: 4]) );
      24: `SEND_LCD( 55`US, hex2char(a[ 3: 0]) );
      25: `SEND_LCD( 55`US, 9'h0C0 ); // DDRAM address=40
      26: `SEND_LCD( 55`US, hex2char(b[63:60]) );
      27: `SEND_LCD( 55`US, hex2char(b[59:56]) );
      28: `SEND_LCD( 55`US, hex2char(b[55:52]) );
      29: `SEND_LCD( 55`US, hex2char(b[51:48]) );
      30: `SEND_LCD( 55`US, hex2char(b[47:44]) );
      31: `SEND_LCD( 55`US, hex2char(b[43:40]) );
      32: `SEND_LCD( 55`US, hex2char(b[39:36]) );
      33: `SEND_LCD( 55`US, hex2char(b[35:32]) );
      34: `SEND_LCD( 55`US, hex2char(b[31:28]) );
      35: `SEND_LCD( 55`US, hex2char(b[27:24]) );
      36: `SEND_LCD( 55`US, hex2char(b[23:20]) );
      37: `SEND_LCD( 55`US, hex2char(b[19:16]) );
      38: `SEND_LCD( 55`US, hex2char(b[15:12]) );
      39: `SEND_LCD( 55`US, hex2char(b[11: 8]) );
      40: `SEND_LCD( 55`US, hex2char(b[ 7: 4]) );
      41: `SEND_LCD( 55`US, hex2char(b[ 3: 0]) );
 default: `SEND_LCD( 55`US, 9'h120 ); // space
endcase
parameter idx_loop=8;
parameter idx_max=41;

endmodule

