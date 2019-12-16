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
// #define CPC_READ_DELAY  FAST_CPC_GETTERS ? _delay_us(50) :  _delay_ms(50) 

// all output 
#define CONFIGURE_TO_CPC_INPUT     DDRA  = 0b11111111 

// all input, no pullups 
#define CONFIGURE_FROM_CPC_OUTPUT  DDRD  = 0b00000000; PORTD  = 0b00000000 

//
// Master DATABUS MACRO 
// 

static volatile uint8_t synchro = 0; 

#define DATA_TO_CPC(arg)    TO_CPC_INPUT = arg 
#define DATA_FROM_CPC(arg)  arg = FROM_CPC_OUTPUT 

//
// Virtual SSA1 PINs - MUST BE ON TO_CPC_INPUT !
// 

#define _LRQ PA6 
#define SBY  PA7  

#define NATIVE_SBY  PA5

//
// DKtronics Signals 
// 

#define dk_speech_idle_loadme        TO_CPC_INPUT = 0 
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

#define speech_native_ready      synchro = 0; setBit(synchro, NATIVE_SBY); DATA_TO_CPC(synchro) 
#define speech_native_busy       synchro = 0; DATA_TO_CPC(synchro) 

//
// CPC Data WRite - From Address Decoder! Trigger 
// 

#define IOREQ_PIN      PINC

#define IOREQ_WRITE    PC7 
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
#define READY_LED    PC0
#define TRANSMIT_LED PC1

// PC2 = Amdrum, PC1 = LED, PC0 = LED, 1 = OUTPUT
#define CONFIGURE_LEDS DDRC = 0b00000111 

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

// ================================
// END PINS 
// 

