`default_nettype none
//`define ENABLE_HPS
`define SWIJTAG

`include "vJTAG_interface.sv"
`include "top.sv"

module DE0_SOC(

	//////////// ADC //////////
	output logic          		ADC_CONVST,
	output logic          		ADC_SCK,
	output logic          		ADC_SDI,
	input  logic           		ADC_SDO,
	//////////// CLOCK //////////
	input  logic           		FPGA_CLK1_50,
	input  logic           		FPGA_CLK2_50,
	input  logic           		FPGA_CLK3_50,
	
`ifdef ENABLE_HPS
	//////////// HPS //////////
	inout 	          		HPS_CONV_USB_N,
	output logic    [14:0]		HPS_DDR3_ADDR,
	output logic     [2:0]		HPS_DDR3_BA,
	output logic          		HPS_DDR3_CAS_N,
	output logic          		HPS_DDR3_CK_N,
	output logic          		HPS_DDR3_CK_P,
	output logic          		HPS_DDR3_CKE,
	output logic          		HPS_DDR3_CS_N,
	output logic     [3:0]		HPS_DDR3_DM,
	inout 	        [31:0]		HPS_DDR3_DQ,
	inout 	         [3:0]		HPS_DDR3_DQS_N,
	inout 	         [3:0]		HPS_DDR3_DQS_P,
	output logic          		HPS_DDR3_ODT,
	output logic          		HPS_DDR3_RAS_N,
	output logic          		HPS_DDR3_RESET_N,
	input  logic           		HPS_DDR3_RZQ,
	output logic          		HPS_DDR3_WE_N,
	output logic          		HPS_ENET_GTX_CLK,
	inout 	          		HPS_ENET_INT_N,
	output logic          		HPS_ENET_MDC,
	inout 	          		HPS_ENET_MDIO,
	input  logic           		HPS_ENET_RX_CLK,
	input  logic      [3:0]		HPS_ENET_RX_DATA,
	input  logic           		HPS_ENET_RX_DV,
	output logic      [3:0]		HPS_ENET_TX_DATA,
	output logic          		HPS_ENET_TX_EN,
	inout 	          		HPS_GSENSOR_INT,
	inout 	          		HPS_I2C0_SCLK,
	inout 	          		HPS_I2C0_SDAT,
	inout 	          		HPS_I2C1_SCLK,
	inout 	          		HPS_I2C1_SDAT,
	inout 	         		HPS_KEY,
	inout 	          		HPS_LED,
	inout 	          		HPS_LTC_GPIO,
	output logic          		HPS_SD_CLK,
	inout 	          		HPS_SD_CMD,
	inout 	          [3:0]		HPS_SD_DATA,
	output logic          		HPS_SPIM_CLK,
	input  logic           		HPS_SPIM_MISO,
	output logic          		HPS_SPIM_MOSI,
	inout 		          		HPS_SPIM_SS,
	input  logic           		HPS_UART_RX,
	output logic          		HPS_UART_TX,
	input  logic           		HPS_USB_CLKOUT,
	inout 	          [7:0]		HPS_USB_DATA,
	input  logic           		HPS_USB_DIR,
	input  logic           		HPS_USB_NXT,
	output logic          		HPS_USB_STP,
`endif /*ENABLE_HPS*/

	//////////// KEY //////////
	input  logic         [1:0]	KEY,

	//////////// LED //////////
	output logic          [7:0]	LED,

	//////////// SW //////////
	input  logic          [3:0]	SW,

	//////////// GPIO_0, GPIO connect to GPIO Default //////////
	inout 		     [35:0]		GPIO_0,

	//////////// GPIO_1, GPIO connect to GPIO Default //////////
	inout 		      [35:0]		GPIO_1
);

logic [7:0] SWI; // chaves pretas
logic [7:0] SWI_JTAG; // chaves pretas pelo JTAG
`ifdef SWIJTAG
always_comb SWI <= SWI_JTAG;
`else
always_comb SWI <= { GPIO_0[ 1], GPIO_0[ 3], GPIO_0[ 5], GPIO_0[ 7],
                     GPIO_0[29], GPIO_0[31], GPIO_0[33], GPIO_0[35] };
`endif

logic [7:0] SEG; // display de 7 segmentos com ponto
always_comb { GPIO_0[23], GPIO_0[11], GPIO_0[13], GPIO_0[15], 
              GPIO_0[17], GPIO_0[19], GPIO_0[27], GPIO_0[25] } <= ~SEG;

// clock lento //
parameter divide_by=100000000;  // divisor de clock, igual ao arquivo .sdc
// obs.: divide_by=1 nao funciona,
//       neste caso use FPGA_CLK1_50 diretamente no lugar de CLOCK_DIV
//       e verifique o setup slack no arquivo DE0_SOC.sta.summary
logic [$clog2(divide_by)-1:0] clock_count;  // contador para o divisor de clock
logic CLOCK_DIV;  // sinal de clock divido para ser referenciado no arquivo .sdc
always_ff @(posedge FPGA_CLK1_50) begin
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

lcd_64bit (.clk(FPGA_CLK1_50), .reset(~KEY[0]),
           .a(lcd_a), .b(lcd_b),
     .LCD_RS(GPIO_1[21]), .LCD_E(GPIO_1[17]),
     .LCD_D({ GPIO_1[ 3], GPIO_1[ 5], GPIO_1[ 7], GPIO_1[ 9],
              GPIO_1[33], GPIO_1[11], GPIO_1[13], GPIO_1[15] }));

always_comb begin
   GPIO_1[ 1] <= 1; // LCD backlight anode
   GPIO_1[23] <= 0; // LCD RW wired to GND on connector
end

vJTAG_interface(.*);
top (.clk_2(CLOCK_DIV), .*);

endmodule

