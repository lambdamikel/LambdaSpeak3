//
// PINS 
// ================================  

//
// TTS Epson IC Part
// 

#define CS   PB3
#define MOSI PB5
#define MISO PB6
#define SCK  PB7

#define TTS_INPUT  PINB
#define TTS_OUTPUT PORTB
#define TTS_DDR    DDRB

#define S1V30120_RDY    PB0
#define S1V30120_RST    PB2
#define S1V30120_CS     PB3

//
//
//

#define EEPROM_INPUT  PINB
#define EEPROM_OUTPUT PORTB
#define EEPROM_CS     PB3 

// All OUTPUT, TTS ClickBoard control, except SCK, 
// and RDY = PB0 and _Z80_READY = PB1 INPUT!  -> 0 = INPUT, 1 = OUTPUT

#define CONFIGURE_TTS_INOUT DDRB  = 0b11111100 

//
// Z80 
// 

#define _Z80_READY PB1 
#define z80_run  DDRB = 0b11111100 
#define z80_halt DDRB = 0b11111110, clearBit(PORTB, _Z80_READY) 

//
// CPC Databus IO 
// 

#define FROM_CPC_OUTPUT PIND 
#define TO_CPC_INPUT    PORTA 

// all output 
#define CONFIGURE_TO_CPC_INPUT     DDRA  = 0b11111111 

// all input, no pullups 
#define CONFIGURE_FROM_CPC_OUTPUT  DDRD  = 0b00000000; PORTD  = 0b00000000 

//
// Master DATABUS MACRO 
// 

// static volatile uint8_t synchro = 0; 

#define DATA_TO_CPC(arg)    TO_CPC_INPUT = arg 
#define DATA_FROM_CPC(arg)  arg = FROM_CPC_OUTPUT 

//
// Virtual SSA1 PINs - MUST BE ON TO_CPC_INPUT !
// 

#define _LRQ PA6 
#define SBY  PA7  

// #define NATIVE_SBY  PA5
// 2^PA5 = 32: &20
#define NATIVE_SBY_VAL 32  

#define SERIAL_SBY_VAL 16 

//
// DKtronics Signals 
// 

#define dk_speech_idle_loadme        TO_CPC_INPUT = 0; 
#define dk_speech_busy               setBit(TO_CPC_INPUT, SBY)

//
// SSA1 Signals 
// 


#define speech_idle_loadme      clearBit(TO_CPC_INPUT, _LRQ),   setBit(TO_CPC_INPUT, SBY)  
#define speech_busy               setBit(TO_CPC_INPUT, _LRQ), clearBit(TO_CPC_INPUT, SBY)  
#define speech_speaking_loadme  clearBit(TO_CPC_INPUT, _LRQ), clearBit(TO_CPC_INPUT, SBY)  


//#define speech_idle_loadme       synchro = 0; setBit(synchro, SBY);  DATA_TO_CPC(synchro) 
//#define speech_busy              synchro = 0; setBit(synchro, _LRQ); DATA_TO_CPC(synchro) 
//#define speech_speaking_loadme   synchro = 0; DATA_TO_CPC(synchro) 

//
// Native Signals - Can be used to determine if LambdaSpeak is still speaking, from CPC side 
// Only makes sense in non-blocking mode of course 
// In blocking mode, the CPC ist halted by pulling Z80 Wait to ground!
// 

//#define speech_native_ready      synchro = 0; setBit(synchro, NATIVE_SBY); DATA_TO_CPC(synchro) 
#define speech_native_ready        DATA_TO_CPC(NATIVE_SBY_VAL) 
#define serial_ready               DATA_TO_CPC(SERIAL_SBY_VAL) 
//#define speech_native_busy       synchro = 0; DATA_TO_CPC(synchro) 
#define speech_native_busy         DATA_TO_CPC(0) 
#define serial_busy                DATA_TO_CPC(0) 

//
// CPC Data WRite - From Address Decoder! Trigger 
// 

#define IOREQ_PIN      PINC

#define IOREQ_WRITE    PC7 

// for DKtronics, ONLY LS 2.0, NOT 1.8, NOT 1.5: 
// note: PC6 is RESET
// in LS 2.1, we need one more output, so this is again shared with IOREQ_WRITE: 
#define IOREQ_WRITE_DK PC7 

//
// Soft Reset Button and LEDs 
// 

// PC6 = D22 
#define SOFT_RESET_INT PCINT22 
#define SOFT_RESET_INT_VEC PCINT2_vect
#define SOFT_RESET_PIN PINC
#define SOFT_RESET_PIN_NUMBER PC6

// pullup for reset!
#define CONFIGURE_RESET PORTC |= 0b01000000 

//
//
// 

#define LED_PORT_READY    PORTC
#define LED_PORT_TRANSMIT PORTC

// PC1 = LED, PC0 = LED, 

#define READY_LED    PC0
#define TRANSMIT_LED PC1

// Status flags für CPLD - 2 bit decoder -> 4 LEDs 
// PC2 = Amdrum ON, PC5 = SP0256 ON, 
// PC4 = SSA1_MODE, PC3 = DK_MODE  
// 

// only PC6 and PC7 ARE INPUTS (RESET FROM BUTTON AND WRITE REQ FROM CPLD) 

// 1 = OUTPUT 

#define CONFIGURE_LEDS DDRC = 0b00111111 
#define CONFIGURE_SP0  DDRC = 0b00111111 

//
// PCM AUDIO 
//

#define PCM_PIN PB4 
#define CONFIGURE_PCM DDRB |= ( 1 << PB4); // = OC0B = PB4 = AMDRUM AUDIO OUTPUT

//
//
//

#define AMDRUM_PORT    PORTC 
#define AMDRUM_ENABLED PC2 

//
// SP0256 IN/OUT
// 

// to CPLD AND SP0256 CHIP: 
#define SP0256_ENABLE_PORT  PORTC 
// hardware enable for SP0256 
#define SP0256_ENABLED      PC5 

//
// GENERAL MODE FLAG - TO CPLD, SSA1 or DK 
// THIS APPLIES BOTH THE SP0256 AND EPSON BASED MODES!!
// 

// to CPLD: 
#define SSA1_MODE    PC4
// to CPLD: 
#define DK_MODE      PC3 


//
// CPLD LEDS:
// SSA1 DKTRONICS SP0256 AMDRUM 
// 3-to-4 DECODER (SSA1_MODE, DK_MODE, SP0256_ENABLED, AMDRUM_ENABLED 
//

// 
// SP0256 DATA PORT
// 

#define SP0256_DATA_PORT    PORTA 

//
//
//

#define DATA_TO_SP0(arg)      SP0256_DATA_PORT = arg 
#define STATUS_FROM_SP0(arg)  arg = SP0256_INPUT_PIN 

//
//
// 

// ================================
// END PINS 
// 
