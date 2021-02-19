`default_nettype none
`define SWIJTAG

`include "vJTAG_interface.sv"
`include "top.sv"

module DE0_Nano(

//////////// clock //////////
input logic                         CLOCK_50,

//////////// LED //////////
output logic             [7:0]      LED,

//////////// KEY //////////
input logic              [1:0]      KEY,

//////////// SW //////////
input logic              [3:0]      SW,

//////////// SDRAM //////////
output logic            [12:0]      DRAM_ADDR,
output logic             [1:0]      DRAM_BA,
output logic                        DRAM_CAS_N,
output logic                        DRAM_CKE,
output logic                        DRAM_CLK,
output logic                        DRAM_CS_N,
inout                   [15:0]      DRAM_DQ,
output logic             [1:0]      DRAM_DQM,
output logic                        DRAM_RAS_N,
output logic                        DRAM_WE_N,

//////////// EPCS //////////
output logic                        EPCS_ASDO,
input  logic                        EPCS_DATA0,
output logic                        EPCS_DCLK,
output logic                        EPCS_NCSO,

//////////// Accelerometer and EEPROM //////////
output logic                        G_SENSOR_CS_N,
input  logic                        G_SENSOR_INT,
output logic                        I2C_SCLK,
inout  logic                        I2C_SDAT,

//////////// ADC //////////
output logic                        ADC_CS_N,
output logic                        ADC_SADDR,
output logic                        ADC_SCLK,
input logic                         ADC_SDAT,

//////////// 2x13 GPIO Header //////////
inout                   [12:0]      GPIO_2,
input logic              [2:0]      GPIO_2_IN,

//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
inout                   [33:0]      GPIO_0,
input logic              [1:0]      GPIO_0_IN,

//////////// GPIO_1, GPIO_1 connect to GPIO Default //////////
inout                   [33:0]      GPIO_1,
input logic              [1:0]      GPIO_1_IN
);


logic [7:0] SWI; // chaves pretas
logic [7:0] SWI_JTAG; // chaves pretas pelo JTAG
`ifdef SWIJTAG
always_comb SWI <= SWI_JTAG;
`else
always_comb SWI <= {GPIO_0[4],GPIO_0[6],GPIO_0[8],GPIO_0[5],GPIO_0[21],GPIO_0[23],GPIO_0[29],GPIO_0[18]};
`endif

logic [7:0] SEG; // display de 7 segmentos com ponto
always_comb {GPIO_0[27],GPIO_0[3],GPIO_0[1],GPIO_0[0],GPIO_0[2],GPIO_0[25],GPIO_0[31],GPIO_0[33]} <= SEG;

// clock lento 
parameter divide_by=100000000;  // divisor de clock, igual ao arquivo .sdc
// obs.: divide_by=1 nao funciona,
//       neste caso use CLOCK_50 diretamente no lugar de CLOCK_DIV
//       e verifique o setup slack no arquivo DE0_Nano.sta.summary
logic [$clog2(divide_by)-1:0] clock_count;  // contador para o divisor de clock
logic CLOCK_DIV;  // sinal de clock divido para ser referenciado no arquivo .sdc
always_ff @(posedge CLOCK_50) begin
  if(clock_count >= divide_by-1) clock_count <= 0;
  else clock_count <= clock_count + 1; 
  CLOCK_DIV <= clock_count[$clog2(divide_by)-1];
end

logic [NBITS_LCD-1:0] lcd_a, lcd_b;
logic [NINSTR_BITS-1:0] lcd_instruction;
logic [NBITS_TOP-1:0] lcd_registrador [0:NREGS_TOP-1];
logic [NBITS_TOP-1:0] lcd_pc, lcd_SrcA, lcd_SrcB,
                      lcd_ALUResult, lcd_Result, lcd_WriteData, lcd_ReadData;
logic lcd_MemWrite, lcd_Branch, lcd_MemtoReg, lcd_RegWrite;

lcd_64bit (.clk(CLOCK_50), .reset(~KEY[0]),
           .a(lcd_a), .b(lcd_b),
     .LCD_RS(GPIO_1[19]), .LCD_E(GPIO_1[15]),
     .LCD_D({ GPIO_1[ 1], GPIO_1[ 3], GPIO_1[ 5], GPIO_1[ 7],
              GPIO_1[31], GPIO_1[ 9], GPIO_1[11], GPIO_1[13] }));
always_comb begin
   GPIO_1[ 0] <= 1; // LCD backlight anode
   GPIO_1[21] <= 0; // LCD RW wired to GND on connector
end

vJTAG_interface(.SWI_JTAG(SWI_JTAG), .LED(LED), .SEG(SEG), .lcd_a(lcd_a), .lcd_b(lcd_b),
     .lcd_pc(lcd_pc), .lcd_instruction(lcd_instruction), .lcd_SrcA(lcd_SrcA), .lcd_SrcB(lcd_SrcB),
     .lcd_ALUResult(lcd_ALUResult), .lcd_Result(lcd_Result), .lcd_WriteData(lcd_WriteData),
     .lcd_ReadData(lcd_ReadData), .lcd_MemWrite(lcd_MemWrite), .lcd_Branch(lcd_Branch), 
     .lcd_MemtoReg(lcd_MemtoReg), .lcd_RegWrite(lcd_RegWrite));
top (.clk_2(CLOCK_DIV), .SWI(SWI),       .LED(LED), .SEG(SEG), .lcd_a(lcd_a), .lcd_b(lcd_b),
     .lcd_pc(lcd_pc), .lcd_instruction(lcd_instruction), .lcd_SrcA(lcd_SrcA), .lcd_SrcB(lcd_SrcB),
     .lcd_ALUResult(lcd_ALUResult), .lcd_Result(lcd_Result), .lcd_WriteData(lcd_WriteData),
     .lcd_ReadData(lcd_ReadData), .lcd_MemWrite(lcd_MemWrite), .lcd_Branch(lcd_Branch), 
     .lcd_MemtoReg(lcd_MemtoReg), .lcd_RegWrite(lcd_RegWrite));

endmodule

