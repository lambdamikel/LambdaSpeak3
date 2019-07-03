void process_control(uint8_t control_byte) {

  LEDS_ON; 

  stop();  

  if (NON_BLOCK_CONFIRMATIONS) {
    z80_run;
  }

  if ( CUR_MODE == SSA1_SPO_M || CUR_MODE == DKTRONICS_SPO_M) {      

    // in this mode, we cannot use any getter methods because of CPLD port rerouting 
    // also, the Epson related commands make no sense, so it would give a wrong impression
    // the RTC cannot be used for same reason (getter methods)
    // USART is ok, because it starts its own loop anyhow 

    switch ( control_byte ) {
 
    case 0xFF : process_reset(); break; 

#ifdef LS300
    case 0xFE : eeprom_pcm_upload_mode(1); break; 
    case 0xFD : eeprom_pcm_play_mode(4); break; 
    case 0xFC : eeprom_pcm_play_mode(3); break; 
    case 0xFB : eeprom_pcm_play_mode(2); break; 
    case 0xFA : eeprom_pcm_play_mode(1); break; 

    case 0xF9 : eeprom_pcm_upload_mode(0); break; 
    case 0xF8 : eeprom_pcm_clear_all(); break; 
    case 0xF7 : eeprom_get_data(); break; 

      // case 0xF6 : eeprom_get_id(); break; 

    case 0xF5 : eeprom_full_check(); break; 
    case 0xF4 : non_blocking_confirmations(); break; 
    case 0xF3 : blocking_confirmations(); break;  
    case 0xF2 : get_full_mode(); break; 
    case 0xF1 : usart_mode_loop(); break; 
    case 0xF0 : next_byte_to_LEDs(); break; 
#endif 

    case 0xEF : native_mode_epson(); break; 
    case 0xEE : native_mode_dectalk(); break; 
    case 0xED : ssa1_mode(); break; 
    case 0xEC : dktronics_mode(); break; 
    case 0xE3 : amdrum_mode(); break; 
#ifndef LS195 
    case 0xE2 : ssa1_spo_mode(); break; 
    case 0xE1 : dktronics_spo_mode(); break; 
#endif 

    case 0xC8 : speak_copyright_note(); break; 
    case 0xC7 : speak_hal9000_quote(); break; 
    case 0xC6 : sing_daisy(); break; 
    case 0xC5 : echo_test_program(); break; 
      //case 0xC4 : echo_test_program_dk(); break; 
    case 0xC3 : announce_cur_mode(); break; 
    case 0xC2 : pcm_test(); break; 

    } 

  } else {
 
    switch ( control_byte ) {
 
    case 0xFF : process_reset(); break; 

#ifdef LS300
    case 0xFE : eeprom_pcm_upload_mode(1); break; 
    case 0xFD : eeprom_pcm_play_mode(4); break; 
    case 0xFC : eeprom_pcm_play_mode(3); break; 
    case 0xFB : eeprom_pcm_play_mode(2); break; 
    case 0xFA : eeprom_pcm_play_mode(1); break;

    case 0xF9 : eeprom_pcm_upload_mode(0); break; 
    case 0xF8 : eeprom_pcm_clear_all(); break; 
    case 0xF7 : eeprom_get_data(); break; 

      // case 0xF6 : eeprom_get_id(); break; 

    case 0xF5 : eeprom_full_check(); break; 
    case 0xF4 : non_blocking_confirmations(); break; 
    case 0xF3 : blocking_confirmations(); break;  
    case 0xF2 : get_full_mode(); break; 
    case 0xF1 : usart_mode_loop(); break; 
    case 0xF0 : next_byte_to_LEDs(); break; 
#endif 

    case 0xEF : native_mode_epson(); break; 
    case 0xEE : native_mode_dectalk(); break; 
    case 0xED : ssa1_mode(); break; 
    case 0xEC : dktronics_mode(); break; 
    case 0xEB : non_blocking(); break; 
    case 0xEA : blocking(); break; 
    case 0xE9 : confirmations_on(); break;  
    case 0xE8 : confirmations_off(); break;   
    case 0xE7 : english(); break; 
    case 0xE6 : spanish(); break; 
    case 0xE5 : fast_getters(); break; 
    case 0xE4 : slow_getters(); break; 
    case 0xE3 : amdrum_mode(); break; 

#ifndef LS195 
    case 0xE2 : ssa1_spo_mode(); break; 
    case 0xE1 : dktronics_spo_mode(); break; 
#endif

    case 0xDF : stop_command(); break;  // will never be reached, because of blocking - only for documentation!!
    case 0xDE : flush_command(); break;

#ifdef RTC
    case 0xDD : i2c_speak_time(); break; 
    case 0xDC : i2c_speak_date(); break; 
    case 0xDB : i2c_set_time(); break; 
    case 0xDA : i2c_set_date(); break;  
    case 0xD3 ... 0xD9 : i2c_get_clock_reg(control_byte - 0xD3); break;       
    case 0xD2 : i2c_get_temp(); break;  
    case 0xD1 : i2c_speak_temp(); break; 
#endif 

    case 0xCF : get_mode(); break; 
    case 0xCE : get_volume(); break; 
    case 0xCD : get_voice(); break; 
    case 0xCC : get_rate(); break; 
    case 0xCB : get_language(); break; 
    case 0xCA : get_delay(); break; 
    case 0xC9 : get_version(); break; 
    case 0xC8 : speak_copyright_note(); break; 
    case 0xC7 : speak_hal9000_quote(); break; 
    case 0xC6 : sing_daisy(); break; 
    case 0xC5 : echo_test_program(); break; 
      // case 0xC4 : echo_test_program_dk(); break; 
    case 0xC3 : announce_cur_mode(); break; 
    case 0xC2 : pcm_test(); break; 

    case 0xB0 : set_voice_default(); break;
    case 0xB1 ... 0xBD : set_voice( control_byte - 0xB0); break; 

    case 0xA0 : set_volume_default(); break;
    case 0xA1 ... 0xAF : set_volume( control_byte - 0xA0); break;

    case 0x90 : set_rate_default(); break;
    case 0x91 ... 0x9F : set_rate( control_byte - 0x90); break;

    case 0x80 : set_buffer_delay_default(); break;
    case 0x81 ... 0x8F : set_buffer_delay( control_byte - 0x80 ); break;

    }

    LEDS_OFF; 
  
    proceed(); 

    return; 

  }
}
