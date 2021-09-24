module inst (
        input logic  [NBITS_TOP-1:0]  address,
        input logic   clock,
        output logic  [31:0]  q);

  int data[1<<(NBITS_TOP-2)];

  initial begin
    int fd, addr, ins;
    fd = $fopen("inst.objdump","r");
    if(fd)
      for(addr=0; addr<(1<<(NBITS_TOP-2)); addr++) begin
         $fscanf(fd,"%h",ins);
         if($feof(fd)) data[addr] = 0; else begin
           ins = {ins[7:0],ins[15:8],ins[23:16],ins[31:24]}; // correct endianess
           $fdisplay(32'h8000_0002,"%h",ins);
           data[addr] = ins;
         end
      end
    else begin
      fd = $fopen("inst.101","r");
      if(fd) begin
        byte c, has_ins;
        has_ins=0;
        do begin
          c = $fgetc(fd);
          case(c)
            ";": begin 
                   do c = $fgetc(fd);
                   while(c!=10);
                   if(has_ins) begin 
                     $fdisplay(32'h8000_0002,"%h",ins);
                     data[addr] = ins;
                     addr++;
                     ins = 0;
                     has_ins=0;
                   end
                 end
            "0": begin
                   ins = ins<<1;
                   has_ins++;
                 end
            "1": begin
                   ins = ins<<1 | 1;
                   has_ins++;
                end
             10: if(has_ins) begin 
                   $display("%h",ins);
                   data[addr] = ins;
                   addr++;
                   ins = 0;
                   has_ins=0;
                 end
          endcase
        end while(!$feof(fd));
      end  
    end
  end

  always @(posedge clock) q <= data[address[NBITS_TOP-1:2]];

endmodule

