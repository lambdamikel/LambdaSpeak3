`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: LambdaSpeak
// Engineer: Michael Wessel
// 
// Create Date:    01:43:46 09/11/2018
// Design Name: 
// Module Name:    Main 
// Project Name: LambdaSpeak 3
// Target Devices: XC9572 7PC84
// Tool versions: Xilinx WebPACK ISE 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Main(

        /// from CPC: 
        i_IORQ,
        i_RD,
        i_WR,
        
        // turned on when in AMDRUM mode OR when in EPSON MODE (OFF FOR SPO256)  
        i_AMDRUM_OR_EPSON_ON, 
        // SPO256 enable 
        i_SPO256_ON,    
        
        // mode selector: for EPSON AND FOR SPO256! 
        i_SSA1_MODE, 
        i_DKTRONICS_MODE, 

        // from SPO256 controlled ATMega outputs for LEDs
        // which get by ATMega software connected from input
        // to LED output for STB and LOAD from SPO256 OUTPUT 
        i_SPO256_SBY, 
        i_SPO256__LRQ, 
        
        iADR,
        ioCPC_DATA, 
        
        // to ATMega / from ATMega Data 
        
        iATMEGA_DATA, 
        oATMEGA_DATA,           
        
        // CPC IOReq Write to FBEE or FBFE or FFxx
        oSPEECH_WRITE , 
        
        // LEDs
        
         oEPSON_ON ,
         oAMDRUM_ON , 
         oSPO256_ON , 
         
         oSSA1_MODE , 
         oDK_MODE , 
         
         // SPI CHP SELECT FROM AVR PB4 -> ROUTE TO TTS OR EEPROM 
         
         i_CHIP_SELECT , 
         o_EPSON_SELECT , 
         o_EEPROM_SELECT ,
         
         // EPSON SPEECH BOARD SLAVE OUT, BECAUSE SLAVE OUT IS NOT TRI-STATE! HAVE TO DO IT MYSELF!
         
         i_EPSON_SLAVE_OUT , 
         o_EPSON_SLAVE_OUT ,
         
         // SERIAL -> CPLD
         // from CPLD from/to ATMega (PD0, PD1) 
         
         oSERIAL_RX , 
         iSERIAL_TX ,

         // from CPLD to/from Outside World      
         
         iRX , 
         oTX 
        
);

input [15:0] iADR; 
input i_IORQ; 
input i_RD; 
input i_WR; 

input i_CHIP_SELECT; 
output o_EPSON_SELECT; 
output o_EEPROM_SELECT; 

input  i_EPSON_SLAVE_OUT; 
output o_EPSON_SLAVE_OUT; 

input i_AMDRUM_OR_EPSON_ON; 
input i_SPO256_ON; 

input i_SSA1_MODE; 
input i_DKTRONICS_MODE; 

input i_SPO256_SBY; 
input i_SPO256__LRQ; 

output oSPEECH_WRITE; 

inout  [7:0] ioCPC_DATA; 

input  [7:0] iATMEGA_DATA; 
output [7:0] oATMEGA_DATA; 

 
output oSERIAL_RX ;
input  iSERIAL_TX ;  
         
input iRX ; 
output oTX  ;  

wire iorq = ~ i_IORQ; 
wire iord = ~ i_RD; 
wire iowr = ~ i_WR; 

wire read = iorq & iord; 
wire write = iorq & iowr; 

//
//
// 

wire spo256_ctrl    = i_SPO256_ON ; 
wire ssa1_ctrl      = i_SSA1_MODE ; 
wire dktronics_ctrl = i_DKTRONICS_MODE ; 
wire amdrum_ctrl    = i_AMDRUM_OR_EPSON_ON ; 

//
//
// 

wire ssa1_spo256  = spo256_ctrl & ! amdrum_ctrl & ssa1_ctrl   & ! dktronics_ctrl ;    
wire dk_spo256    = spo256_ctrl & ! amdrum_ctrl & ! ssa1_ctrl &   dktronics_ctrl ;  

wire ssa1_epson   = ! spo256_ctrl & amdrum_ctrl & ssa1_ctrl   & ! dktronics_ctrl ;  
wire dk_epson     = ! spo256_ctrl & amdrum_ctrl & ! ssa1_ctrl &   dktronics_ctrl ;  

wire amdrum       = ! spo256_ctrl & amdrum_ctrl & ! ssa1_ctrl & ! dktronics_ctrl ;  

wire lambda_epson   = ! spo256_ctrl & ! amdrum_ctrl & ! ssa1_ctrl & ! dktronics_ctrl ; 
wire lambda_dectalk = ! spo256_ctrl &   amdrum_ctrl &   ssa1_ctrl &   dktronics_ctrl ; 

// also allow SPO to be one with eeprom: 
wire eeprom_sample_upload =  ! spo256_ctrl & ! amdrum_ctrl & ! ssa1_ctrl &   dktronics_ctrl ; 
wire eeprom_sample_play   =    spo256_ctrl &   amdrum_ctrl &   ssa1_ctrl & ! dktronics_ctrl ; 


//

wire serial_mode = ! spo256_ctrl & ! amdrum_ctrl & ssa1_ctrl & dktronics_ctrl ;  


//
//
//


assign o_EEPROM_SELECT = i_CHIP_SELECT ? 1 : ! ( eeprom_sample_upload | eeprom_sample_play ); 
assign o_EPSON_SELECT  = i_CHIP_SELECT ? 1 :   ( eeprom_sample_upload | eeprom_sample_play ); 

// 
// TRI STATE FOR EPSON SLAVE OUT
// 

assign o_EPSON_SLAVE_OUT = ( eeprom_sample_upload | eeprom_sample_play ) ? 1'bz : i_EPSON_SLAVE_OUT ; 

//
//
// 

wire ssa1_adr1   = iADR[15:0] == 16'hFBEE; 
wire ssa1_adr2   = iADR[15:0] == 16'hFAEE; 
wire   dk_adr   = iADR[15:0] == 16'hFBFE; 
wire amdrum_adr = iADR[15:8] == 8'hFF; 

//
//
// 

// epson speech or serial status read: 
wire oSPEECH_OR_SERIAL_READ      = ( ssa1_adr1 | ssa1_adr2 | dk_adr ) & read & 
                                   ( ssa1_epson | dk_epson | lambda_epson | lambda_dectalk | serial_mode ) ;                                                                            
                                                                                          
wire oSPEECH_READ_SPO_SSA1       = ( ssa1_adr1 | ssa1_adr2 ) & read & ssa1_spo256 ;  

wire oSPEECH_READ_SPO_DKTRONICS  =  dk_adr                   & read & dk_spo256 ;  

// write detected 

wire oSPEECH_WRITE = ( ( ( ssa1_adr1 | ssa1_adr2 | dk_adr ) & ! amdrum ) | 
                       ( amdrum_adr & amdrum ) ) & write;                                                                                                                        

//
// logic 
//

reg [7:0] cpc_data = 0; 
reg [7:0] atmega_data = 0; 
reg [7:0] spo_status_ssa1 = 8'bz; 
reg [7:0] spo_status_dktronics = 8'bz; 


// when decoder_write, store CPC databus byte in cpc_data 
always @(posedge oSPEECH_WRITE ) 
begin
        cpc_data <= ioCPC_DATA; 
end

always @(posedge oSPEECH_OR_SERIAL_READ ) 
begin
        atmega_data <= iATMEGA_DATA; 
end

always @(posedge oSPEECH_READ_SPO_SSA1 ) 
begin
   //spo_status_ssa1[5:0] <= 'bzzzzzz ; 
        spo_status_ssa1[7] <= i_SPO256_SBY ;
        spo_status_ssa1[6] <= i_SPO256__LRQ ; 
end

always @(posedge oSPEECH_READ_SPO_DKTRONICS ) 
begin
   //spo_status_dktronics[5:0] <= 'bzzzzzz ; 
        spo_status_dktronics[6] <= i_SPO256_SBY ;
        spo_status_dktronics[7] <= i_SPO256__LRQ ; 
end

// make the output of the atmega data available to cpc when it wants
// to read it this is either atmega output or atmega spo output
// (processed from spo)

assign ioCPC_DATA    = oSPEECH_OR_SERIAL_READ ? atmega_data : 
                     ( oSPEECH_READ_SPO_SSA1 ? spo_status_ssa1 : 
                     ( oSPEECH_READ_SPO_DKTRONICS ? spo_status_dktronics : 8'bz ) ) ; 

// in case the ATMega has not put the CPLD in serial mode, just make
// the latched CPC databus output (from IOREQ write) available to
// ATMega. If that should put the ATMega in serial mode via command
// byte, then 2 of the inputs of the ATMega (PD0, PD1 = RX0, TX0)
// change from simple inputs to serial USART mode (input, output) -
// hence, the CPLD disconnects cpc_data and puts oATMEGA_DATA into
// high Z mode (disconnected) and at the same time, we connect PD0 to
// iSERIAL_RX, and PD1 to oSERIAL_TX. 

assign oATMEGA_DATA = ! serial_mode ? cpc_data : 8'bz; 

assign oTX = serial_mode ? iSERIAL_TX : 1'bz; 

assign oSERIAL_RX = serial_mode ? iRX : 1'bz; 

//
// output LEDs 
// 

output oEPSON_ON ; 
output oAMDRUM_ON ; 
output oSPO256_ON ; 
output oSSA1_MODE ; 
output oDK_MODE ;  

//
// LEDs 
// 


assign oEPSON_ON  = ssa1_epson | dk_epson | lambda_epson | lambda_dectalk ;
    
assign oSPO256_ON = spo256_ctrl           | eeprom_sample_play ;
    
assign oAMDRUM_ON = amdrum | eeprom_sample_upload | eeprom_sample_play ;
         
assign oSSA1_MODE = ssa1_spo256 | ssa1_epson | eeprom_sample_upload | eeprom_sample_play | lambda_epson ;
                                                         
assign oDK_MODE   = dk_spo256   | dk_epson   | eeprom_sample_upload | eeprom_sample_play | lambda_epson ; 

endmodule

