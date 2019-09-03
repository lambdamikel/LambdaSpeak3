
#ifdef LS300

#define READ_ARGUMENT_FROM_DATABUS(databus) { serial_ready; loop_until_bit_is_set(IOREQ_PIN, IOREQ_WRITE); serial_busy; LAMBDA_EPSON_ON; loop_until_bit_is_clear(IOREQ_PIN, IOREQ_WRITE); delay_us(3); DATA_FROM_CPC(databus); SERIAL_ON; }

#define SEND_TO_CPC_DATABUS(byte) { DATA_TO_CPC(byte); CPC_READ_DELAY; DATA_TO_CPC(0); CPC_READ_DELAY; }


//
//
// 
 
void usart_on0(uint8_t rate, uint8_t width, uint8_t parity, uint8_t stop_bits) {

  SERIAL_BAUDRATE = rate; 
  SERIAL_WIDTH = width; 
  SERIAL_PARITY = parity; 
  SERIAL_STOP_BITS = stop_bits; 
 
  UBRR0H = 0;    
  switch (rate) {
  case 0 : UBRR0H = ( 520 >> 8) & 0xFF; UBRR0L = 520 & 0xFF; SERIAL_RATE = 2400; break; // 2400 
  case 1 : UBRR0H = ( 259 >> 8) & 0xFF; UBRR0L = 259 & 0xFF; SERIAL_RATE = 4800; break; // 4800 
  case 2 : UBRR0L = 129; SERIAL_RATE = 9600; break; // 9600 
  case 3 : UBRR0L = 86; SERIAL_RATE = 14400; break;  // 14400 
  case 4 : UBRR0L = 64; SERIAL_RATE = 19200; break;  // 19200
  case 5 : UBRR0L = 42; SERIAL_RATE = 28800; break;  // 28800
  case 6 : UBRR0L = 39; SERIAL_RATE = 31250; break;  // 31250 MIDI ! NEW
  case 7 : UBRR0L = 32; SERIAL_RATE = 38400; break;  // 38400 
  case 8 : UBRR0L = 21; SERIAL_RATE = 57600; break;  // 57600 
  case 9 : UBRR0L = 15; SERIAL_RATE = 76800; break;  // 76800 
  case 10 : UBRR0L = 10; SERIAL_RATE = 115200; break;  // 115200 
  case 11 : UBRR0L = 5; SERIAL_RATE = 208333; break;  // 208333
  case 12 : UBRR0L = 4; SERIAL_RATE = 250000; break;  // 250000
  case 13 : UBRR0L = 3; SERIAL_RATE = 312500; break;  // 312500
  case 14 : UBRR0L = 2; SERIAL_RATE = 416667; break;  // 416667
  case 15 : UBRR0L = 1; SERIAL_RATE = 625000; break;  // 625000
    // case 15 : UBRR0L = 0; SERIAL_RATE = 1250000; break;  // 1250000

  default :  UBRR0L = 129; SERIAL_RATE = 9600; // 9600 
  }

  UCSR0C = 0; 

  switch (parity) { 
  case 0 :                                         break; // no parity 
  case 1 : UCSR0C |= (1 << UPM01) | (1 << UPM00) ; break; // odd parity 
  case 2 : UCSR0C |= (1 << UPM01)                ; break; // even parity 
  default : break; 
  }

  switch (stop_bits) { 
  case 2 :                         break; // 1 stop bit
  case 1 : UCSR0C |= (1 << USBS0); break; // 2 stop bit
  default : break; 
  }


  switch (width) {
  case 8 : UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); break; // 8bit 
  case 7 : UCSR0C |=                 (1 << UCSZ01); break; // 7bit 
  case 6 : UCSR0C |= (1 << UCSZ00)                ; break; // 6bit 
  case 5 :                                          break; // 5bit 

  default : UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);       // 8bit  

  }

  UCSR0B = 0; 
  
}

//
//
//

void usart_off(void) {    
  UCSR0B = 0;
}

void usart_init(void) {
  usart_off(); 
  usart_on0(SERIAL_BAUDRATE, SERIAL_WIDTH, SERIAL_PARITY, SERIAL_STOP_BITS); 

  UCSR0B |= (1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0); 

}

//
// RX ISR Receive / Buffered
//

ISR(USART0_RX_vect) {  
  
  uint8_t data = UDR0; 
  
  if (usart_ring_buffer) {
    if (usart_input_buffer_index >= SEND_BUFFER_SIZE) {
      usart_input_buffer_index = 0; 
    }    
    send_msg[usart_input_buffer_index] = data; 
    usart_input_buffer_index++;    

  } else {
    
    // normal operation 

    if (usart_input_buffer_index < SEND_BUFFER_SIZE) {
      send_msg[usart_input_buffer_index] = data; 
      usart_input_buffer_index++;
    } else if ( usart_input_buffer_index < TOTAL_BUFFER_SIZE) {
      buffer[usart_input_buffer_index - SEND_BUFFER_SIZE] = data; 
      usart_input_buffer_index++; // might be full now, TOTAL_BUFFER_SIZE reched, further input not buffered until flushed!     
    }
  }
}
 
//
// TX Transmit
// 


void USART_Transmit1( unsigned char data ){
   while ( !( UCSR0A & (1<<UDRE0)) ) {  }; 
   UDR0 = data;   
 } 

void USART_Transmit( unsigned char data ){

  usart_input_buffer_index = 0; 
  cpc_read_cursor = 0;
  USART_Transmit1(data); 
  // while ( !( UCSR0A & (1<<TXC0)) ) {  }; 

} 

void USART_sendBuffer(uint16_t length) {
  
  usart_input_buffer_index = 0; 
  cpc_read_cursor = 0;

  for (int i = 0; i < length; i++) {
    // while ( !( UCSR0A & (1<<UDRE0)) ) { if (bit_is_set(IOREQ_PIN, IOREQ_WRITE)) return; }  
    while ( !( UCSR0A & (1<<UDRE0)) ) { }; 
    if (i < SEND_BUFFER_SIZE) {
      UDR0 = send_msg[i]; 
    } else if ( (i - SEND_BUFFER_SIZE) < SPEECH_BUFFER_SIZE) {
      UDR0 = buffer[i - SEND_BUFFER_SIZE]; 
    }
  }

}

//
//
//

void USART_ISR_Transmit( unsigned char data ){
  databus2 = data;
  UCSR0B |= (1<<UDRIE0); // enable UDRE interrupt  
}  

ISR(USART0_UDRE_vect) {
  UCSR0B &= ~(1<<UDRIE0); // disable UDRE interrupt
  UDR0 = databus2; 
}


/* 
ISR(USART0_TX_vect) {
  // called when transmission is complete - disable TX! 
  usart_isr_tx_off(); 
} 
*/ 

//
//
// 

void usart_mode_loop(void) {

  uint8_t direct_mode = 0;  
  
  usart_ring_buffer = 0; 

  serial_busy;  

  LEDS_OFF; 

  command_confirm("Serial mode. Serial commands start with 255."); 

  usart_input_buffer_index = 0; 
  from_cpc_input_buffer_index = 0; 
  cpc_read_cursor = 0; 
 
  z80_run; 
  sei(); 
  
  usart_init(); 

  SERIAL_ON; 
  
  CUR_MODE = SERIAL_M; 

  stop_timer;

  while (1) {

    serial_busy;   

    while ( !( UCSR0A & (1<<UDRE0)) ) { }; 

    READY_ON;
    READ_ARGUMENT_FROM_DATABUS(databus); 

    // z80_halt;

    if (databus == 255) {

      // command sent! 
      // receive command byte - what to do? 

      // z80_run;

      // sei(); 

      TRANSMIT_ON;
      READ_ARGUMENT_FROM_DATABUS(databus); 
      LEDS_OFF; 

      serial_busy;   

      // cli(); 
      // z80_halt;

      if (databus == 255) {
	
	// escape: 255 255 -> 255 ! 

	if (direct_mode) {
	  USART_Transmit(databus); 
	} else {

	  if (from_cpc_input_buffer_index < SEND_BUFFER_SIZE) {
	    send_msg[ from_cpc_input_buffer_index ] = databus; 
	    from_cpc_input_buffer_index++;
	  } else if ( from_cpc_input_buffer_index < TOTAL_BUFFER_SIZE) {
	    buffer[ from_cpc_input_buffer_index  - SEND_BUFFER_SIZE] = databus; 
	    from_cpc_input_buffer_index++;
	  }

	}
	 
      } else {

	// dispatch, decode command byte
	switch (databus) {

	case 10 : // USART MONITOR OLD - CANNOT TAKE COMMANDS FROM CPC 

	  usart_ring_buffer = 1;  // wrap around! 

	  usart_input_buffer_index = 0; 
	  SERIAL_ON; 

	  usart_init(); 

	  cpc_read_cursor = 0;  
	  LEDS_OFF; 

	  while (1) {
	    
	    TRANSMIT_ON; 
	    loop_until_bit_is_set(IOREQ_PIN, IOREQ_WRITE); 
	    loop_until_bit_is_clear(IOREQ_PIN, IOREQ_WRITE); 
	    TRANSMIT_OFF; 

	    cli(); 

	    databus = cpc_read_cursor != usart_input_buffer_index;
	    DATA_TO_CPC(databus); 
	    	    
	    if (databus) {
	    
	      z80_halt;

	      if ( cpc_read_cursor >= SEND_BUFFER_SIZE) 
		cpc_read_cursor = 0; 

	      databus = send_msg[ cpc_read_cursor ]; 
	      cpc_read_cursor++;

	      z80_run;
	    
	      READY_ON; 
	      loop_until_bit_is_set(IOREQ_PIN, IOREQ_WRITE); 
	      loop_until_bit_is_clear(IOREQ_PIN, IOREQ_WRITE); 	      
	      DATA_TO_CPC(databus); 
	      READY_OFF; 

	      // SOFT MIDI THROUGH: 
	      USART_Transmit1(databus); 
	    	      
	    }	 

	    sei(); 

	  } // end while - cannot exit here! 

	  usart_ring_buffer = 0; 
	  
	  break;	  

	case 50 : // USART MONITOR NEW - WITH COMMANDS FROM CPC 

	  usart_ring_buffer = 1;  // wrap around! 

	  usart_input_buffer_index = 0; 
	  SERIAL_ON; 

	  usart_init(); 

	  cpc_read_cursor = 0;  
	  LEDS_OFF; 

	  direct_mode = 0; // used for sending / buffering! 
	  databus1 = 0; // used for sending / buffering! 

	  while (1) {	  

	    TRANSMIT_ON;  
	    SERIAL_ON; 

	    if (direct_mode) {
	      USART_Transmit1(databus1);	       
	      direct_mode = 0; 
	      // wait for transmit to finish: 
	      // TXC0 becomes 1 when Transmission Complete
	      while ( !( UCSR0A & (1<<TXC0)) ) {  }; 
	      _delay_us(500); 
	    }

	    z80_run;
	    
	    loop_until_bit_is_set(IOREQ_PIN, IOREQ_WRITE); 	    
	    loop_until_bit_is_clear(IOREQ_PIN, IOREQ_WRITE); 	      

	    cli(); // protect from interrupts 
	    LAMBDA_EPSON_ON; 
	    DATA_FROM_CPC(databus); 
	    SERIAL_ON;  
	    sei(); 

	    TRANSMIT_OFF; 

	    if (databus == 255) {

	      // read second byte... 255 is escape symbol 
	      READY_ON; 

	      loop_until_bit_is_set(IOREQ_PIN, IOREQ_WRITE); 
	      loop_until_bit_is_clear(IOREQ_PIN, IOREQ_WRITE); 	      

	      z80_halt;

	      cli(); 
	      LAMBDA_EPSON_ON; 
	      DATA_FROM_CPC(databus); 
	      SERIAL_ON;  
	      sei(); 

	      READY_OFF; 	      

	      if (databus == 20) {
		z80_run;
		break; 
	      } else if (databus == 255) {
		direct_mode = 1; 
		databus1 = 255; 
	      } else {
		// do nothing 
	      }
	    } else {
	      direct_mode = 1; 
	      databus1 = databus; 
	    }

	    databus = cpc_read_cursor != usart_input_buffer_index;
	    DATA_TO_CPC(databus); 
	    	    
	    if (databus) {
	    
	      if ( cpc_read_cursor >= SEND_BUFFER_SIZE) 
		cpc_read_cursor = 0; 

	      databus = send_msg[ cpc_read_cursor ]; 
	      cpc_read_cursor++;

	      z80_run;

	      READY_ON; 
	      loop_until_bit_is_set(IOREQ_PIN, IOREQ_WRITE); 
	      loop_until_bit_is_clear(IOREQ_PIN, IOREQ_WRITE); 	      
	      DATA_TO_CPC(databus); 
	      READY_OFF; 

	    }	 
	    
	  } // end while - cannot exit here! 

	  usart_ring_buffer = 0; 
	  
	  break;	  


	case 1 :  // write USART single byte 

	  // z80_run;

	  READ_ARGUMENT_FROM_DATABUS(databus); 

	  // z80_halt; 

	  USART_Transmit(databus); 

	  break; 

	case 2 :  // print buffer to serial 

	  USART_sendBuffer(from_cpc_input_buffer_index); 
	  from_cpc_input_buffer_index = 0; 

	  break; 
	  
	case 3 : // ask for low byte number of bytes in USART input buffer

	  SEND_TO_CPC_DATABUS( usart_input_buffer_index & 0xFF); 

	  break; 

	case 4 : // ask for high byte number of bytes in USART input buffer

	  SEND_TO_CPC_DATABUS( usart_input_buffer_index >> 8); 

	  break; 

	case 5 : // ask if buffer is full 

	  SEND_TO_CPC_DATABUS(usart_input_buffer_index == TOTAL_BUFFER_SIZE); 

	  break; 

	case 6 : // flush receive buffer 

	  usart_input_buffer_index = 0; 
	  cpc_read_cursor = 0; 

	  break; 
	  
	case 7 : // check if byte available 

	  // SEND_TO_CPC_DATABUS( cpc_read_cursor < buffer_index  ); 

	  SEND_TO_CPC_DATABUS( cpc_read_cursor != usart_input_buffer_index  ); 

	  break; 

	case 8 : // get byte for CPC at current USART input buffer position

	  databus = 0; 	    

	  // if (cpc_read_cursor >= 0 && cpc_read_cursor < buffer_index ) {

	  if (cpc_read_cursor != usart_input_buffer_index ) {

	    if ( cpc_read_cursor < SEND_BUFFER_SIZE) {
	      databus = send_msg[ cpc_read_cursor ]; 
	    } else if ( cpc_read_cursor < TOTAL_BUFFER_SIZE) {
	      databus = buffer[ cpc_read_cursor - SEND_BUFFER_SIZE];
	    }
	  }

	  SEND_TO_CPC_DATABUS( databus); 
	    
	  break; 

	case 9 : // get next byte for CPC in USART input buffer

	  databus = 0; 
	  
	  // if (cpc_read_cursor >= 0 && cpc_read_cursor < buffer_index ) {

	  if (cpc_read_cursor != usart_input_buffer_index ) {
	    
	    if ( cpc_read_cursor < SEND_BUFFER_SIZE) {
	      databus = send_msg[ cpc_read_cursor ]; 
	      cpc_read_cursor++;
	    } else if ( cpc_read_cursor < TOTAL_BUFFER_SIZE) {
	      databus = buffer[cpc_read_cursor - SEND_BUFFER_SIZE];
	      cpc_read_cursor++;
	    }
	  }

	  SEND_TO_CPC_DATABUS( databus); 

	  break;

	case 11 : // set cursor to given byte position 

	  // z80_run;

	  READ_ARGUMENT_FROM_DATABUS(databus); 
	  READ_ARGUMENT_FROM_DATABUS(databus1); 

	  // z80_halt; 
	  
	  cpc_read_cursor = databus + (databus1 << 8); 
	  	  
	  break;

	case 12 : // reset CPC cursor 

	  cpc_read_cursor = 0; 
	  
	  break; 

	case 13 : // set CPC cursor to last byte 

	  if (usart_input_buffer_index > 0) {
	    cpc_read_cursor =  usart_input_buffer_index-1; 
	  }
	  
	  break; 

	case 14 :  // check status 

	  SEND_TO_CPC_DATABUS(direct_mode); 

	  break; 

	case 15 : // announce mode!
	  
	  LAMBDA_EPSON_ON; 	  	    
	  sprintf(command_string, "Serial interface active at %lu bauds, %d bits, %d stop bits, and %s parity. %s mode.", 
		  SERIAL_RATE, SERIAL_WIDTH, SERIAL_STOP_BITS, 
		  ( SERIAL_PARITY == 1 ? "odd" : ( SERIAL_PARITY == 2 ? "even" : "no" )),
		  (direct_mode == 1 ? "direct" : "buffered" )); 

	  command_confirm(command_string); 
	  
	  usart_input_buffer_index = 0; 
	  cpc_read_cursor = 0; 
	  SERIAL_ON; 

	  break; 

	case 16 :  // turn on databus direct printing 

	  direct_mode = 1;  

	  break; 

	case 17 :  // turn off databus direct printing  -> print to buffer! 

	  from_cpc_input_buffer_index = 0; 
	  direct_mode = 0; 

	  break; 

	case 20 : 
	  
	  // z80_run; 

	  usart_off(); 	   
	  LAMBDA_EPSON_ON; 	  	    
	  serial_busy;  

	  // BLOCKING = 1; 
	  // NON_BLOCK_CONFIRMATIONS = 0; 
	  command_confirm("Quitting serial mode."); 

	  DATA_TO_CPC(0);   
	  CUR_MODE = START_OVER_SAME_MODE; 
	  // process_reset(); 

	  return; 

	  break;  

	case 30 : // set BAUDRATE

	  // z80_run;

	  READ_ARGUMENT_FROM_DATABUS(SERIAL_BAUDRATE); 

	  // z80_halt;

	  usart_init(); 

	  break; 

	case 31 : // set WIDTH

	  // z80_run; 

	  READ_ARGUMENT_FROM_DATABUS(SERIAL_WIDTH); 

	  // z80_halt; 

	  usart_init(); 

	  break; 

	case 32 : // set PARITY

	  // z80_run; 
	  
	  READ_ARGUMENT_FROM_DATABUS(SERIAL_PARITY); 

	  // z80_halt; 

	  usart_init(); 
 
	  break; 

	case 33 : // set STOP_BITS

	  // z80_run; 
	  
	  READ_ARGUMENT_FROM_DATABUS(SERIAL_STOP_BITS); 

	  // z80_halt;

	  usart_init(); 

	  break; 

	case 55 : // TEST  
	  
	  READ_ARGUMENT_FROM_DATABUS(databus); 
	  DATA_TO_CPC(databus);  
	  
	  break; 

	case 0xF2 : get_full_mode(); break; 

	case 0xC3 : 
	  LAMBDA_EPSON_ON; 	  	    
	  announce_cur_mode(); 
	  SERIAL_ON; 

	  break; 
 
	}
      }

    } else {
	
      // not a control byte, send or buffer: 
	
      if (direct_mode) {

	USART_Transmit(databus); 

      } else

	if (from_cpc_input_buffer_index < SEND_BUFFER_SIZE) {
	  send_msg[ from_cpc_input_buffer_index ] = databus; 
	  from_cpc_input_buffer_index++; 
	} else if ( from_cpc_input_buffer_index < TOTAL_BUFFER_SIZE) {
	  buffer[ from_cpc_input_buffer_index  - SEND_BUFFER_SIZE] = databus; 
	  from_cpc_input_buffer_index++; 
	}    
    }   
  }
}

#endif 

