// http://idlelogiclabs.com/2012/04/15/talking-to-the-de0-nano-using-the-virtual-jtag-interface/

module vJTAG_interface (
        output logic [7:0] SWI_JTAG,
        input logic [NBITS_TOP-1:0] LED,
        input logic [NBITS_TOP-1:0] SEG,
        input logic [NBITS_TOP-1:0] jtag_registrador [0:NREGS_TOP-1],
        input logic [NBITS_LCD-1:0] lcd_a, lcd_b,
        input logic [NINSTR_BITS-1:0] lcd_instruction,
        input logic [NBITS_TOP-1:0] lcd_pc, lcd_SrcA, lcd_SrcB,
           lcd_ALUResult, lcd_Result, lcd_WriteData, lcd_ReadData, 
        input logic lcd_MemWrite, lcd_Branch, lcd_MemtoReg, lcd_RegWrite);

logic tdo, tck, tdi;
logic ir_in;
logic sdr, udr;

vJTAG   (
        .tck(tck),
        .tdi(tdi),
        .tdo(tdo),

        .ir_in(ir_in),

        .virtual_state_sdr(sdr),
        .virtual_state_udr(udr));

logic [7:0] dr; 
logic DR0_bypass_reg; // Safeguard in case bad IR is sent through JTAG 
logic [7:0] DR1; // Date, time and revision DR.  We could make separate Data Registers for each one, but

logic select_DR1;
always_comb select_DR1 = ir_in; //Data Register 1 will collect the new LED Settings

always_ff @(posedge tck)
	begin
		DR0_bypass_reg <= tdi; //Update the Bypass Register Just in case the incoming data is not sent to DR1
		
		if ( sdr )  // VJI is in Shift DR state
			if (select_DR1) //ir_in has been set to choose DR1
					DR1 <= {tdi, DR1[7:1]}; // Shifting in (and out) the data	
	end

logic [2:0] idx;

always_ff @(posedge tck)
   begin
         if (udr) idx <= 0;
         else idx <= idx+1;
   end	
		
//Maintain the TDO Continuity
always_comb
   if (select_DR1) begin
      if (udr)
            tdo <= DR1[0];
      else begin
              if(dr[7:5]=='b000) tdo <= jtag_registrador[dr][idx];
         else if(dr[7:5]=='b001)
            case(dr[4:0])
               'b00000: tdo <= LED[idx];
               'b00001: tdo <= SEG[idx];
               'b00010: tdo <= lcd_pc[idx];
               'b00011: tdo <= lcd_instruction[idx];
               'b00100: tdo <= lcd_instruction[idx+8];
               'b00101: tdo <= lcd_instruction[idx+16];
               'b00110: tdo <= lcd_instruction[idx+24];
               'b00111: tdo <= lcd_SrcA[idx];
               'b01000: tdo <= lcd_SrcB[idx];
               'b01001: tdo <= lcd_ALUResult[idx];
               'b01010: tdo <= lcd_Result[idx];
               'b01011: tdo <= lcd_WriteData[idx];
               'b01100: tdo <= lcd_ReadData[idx];
               'b01101: case(idx)
                           0: tdo <= lcd_MemWrite;
                           1: tdo <= lcd_Branch;
                           2: tdo <= lcd_MemtoReg;
                           3: tdo <= lcd_RegWrite;
                           default: tdo <= 0;
                        endcase
               'b10000: tdo <= lcd_a[idx];
               'b10001: tdo <= lcd_a[idx+8];
               'b10010: tdo <= lcd_a[idx+16];
               'b10011: tdo <= lcd_a[idx+24];
               'b10100: tdo <= lcd_a[idx+32];
               'b10101: tdo <= lcd_a[idx+40];
               'b10110: tdo <= lcd_a[idx+48];
               'b10111: tdo <= lcd_a[idx+56];
               'b11000: tdo <= lcd_b[idx];
               'b11001: tdo <= lcd_b[idx+8];
               'b11010: tdo <= lcd_b[idx+16];
               'b11011: tdo <= lcd_b[idx+24];
               'b11100: tdo <= lcd_b[idx+32];
               'b11101: tdo <= lcd_b[idx+40];
               'b11110: tdo <= lcd_b[idx+48];
               'b11111: tdo <= lcd_b[idx+56];
               default: tdo <= 0;
            endcase
         else tdo <= LED[idx];
      end
   end else tdo <= DR0_bypass_reg;	
	

//The udr signal will assert when the data has been transmitted and it's time to Update the DR
//  so copy it to the Output register. 
//  Note that connecting directly to the DR1 register will cause an unwanted behavior as data is shifted through it
always @(posedge udr) begin
	dr <= DR1;
        // set or reset only one switch at a time
        if(DR1[7:4]=='b0100) SWI_JTAG[DR1[3:1]] <= DR1[0]; 
end

endmodule

