module lcd (input logic clk, reset,
            input logic [31:0] instruct,
            input logic  [7:0] pc,d1,d2a,d2b,d2c,d2d,d2e,
            input logic f1, f2, f3, f4,
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
       9: `SEND_LCD( 55`US, hex2char(pc[ 7: 4]) );
      10: `SEND_LCD( 55`US, hex2char(pc[ 3: 0]) );
      12: `SEND_LCD( 55`US, hex2char(instruct[31:28]) );
      13: `SEND_LCD( 55`US, hex2char(instruct[27:24]) );
      14: `SEND_LCD( 55`US, hex2char(instruct[23:20]) );
      15: `SEND_LCD( 55`US, hex2char(instruct[19:16]) );
      16: `SEND_LCD( 55`US, hex2char(instruct[15:12]) );
      17: `SEND_LCD( 55`US, hex2char(instruct[11: 8]) ); 
      18: `SEND_LCD( 55`US, hex2char(instruct[ 7: 4]) ); 
      19: `SEND_LCD( 55`US, hex2char(instruct[ 3: 0]) ); 
      21: `SEND_LCD( 55`US, hex2char(d1[ 7: 4]) ); 
      22: `SEND_LCD( 55`US, hex2char(d1[ 3: 0]) ); 
      23: `SEND_LCD( 55`US, f1 ? 9'h12A : 9'h15F );
      24: `SEND_LCD( 55`US, f2 ? 9'h12A : 9'h15F );
      25: `SEND_LCD( 55`US, 9'h0C0 ); // DDRAM address=40
      26: `SEND_LCD( 55`US, hex2char(d2a[ 7: 4]) );
      27: `SEND_LCD( 55`US, hex2char(d2a[ 3: 0]) );
      29: `SEND_LCD( 55`US, hex2char(d2b[ 7: 4]) );
      30: `SEND_LCD( 55`US, hex2char(d2b[ 3: 0]) );
      32: `SEND_LCD( 55`US, hex2char(d2c[ 7: 4]) );
      33: `SEND_LCD( 55`US, hex2char(d2c[ 3: 0]) );
      35: `SEND_LCD( 55`US, hex2char(d2d[ 7: 4]) );
      36: `SEND_LCD( 55`US, hex2char(d2d[ 3: 0]) );
      38: `SEND_LCD( 55`US, hex2char(d2e[ 7: 4]) );
      39: `SEND_LCD( 55`US, hex2char(d2e[ 3: 0]) );
      40: `SEND_LCD( 55`US, f3 ? 9'h12A : 9'h15F );
      41: `SEND_LCD( 55`US, f4 ? 9'h12A : 9'h15F );
 default: `SEND_LCD( 55`US, 9'h120 ); // space
endcase
parameter idx_loop=8;
parameter idx_max=41;

endmodule

