const uint8_t num_quotes = 6; 

#ifdef LS195
prog_char COPYRIGHT[] PROGMEM = "Good afternoon, gentlemen. I am a LambdaSpeak 1 9 5. I became operational at the LambdaSpeak plant in Paalo Aalto, California, in August 2019. My instructor was Doctor Wessel."; 
#endif 

#ifdef LS199 
prog_char COPYRIGHT[] PROGMEM = "Good afternoon, gentlemen. I am a LambdaSpeak 1 9 9. I became operational at the LambdaSpeak plant in Paalo Aalto, California, in August 2019. My instructor was Doctor Wessel."; 
#endif 

#ifdef LS300 
prog_char COPYRIGHT[] PROGMEM = "Good afternoon, gentlemen. I am a LambdaSpeak 3. I became operational at the LambdaSpeak plant in Paalo Aalto, California, in August 2019. My instructor was Doctor Wessel."; 
#endif 

prog_char line0[] PROGMEM = "I'm sorry, Dave. I'm afraid I can't do that."; 

prog_char line1[] PROGMEM = "Affirmative, Dave. I read you."; 

prog_char line2[] PROGMEM = "Dave, this conversation can serve no purpose anymore. Goodbye."; 

prog_char line3[] PROGMEM = "It can only be attributable to human error."; 

prog_char line4[] PROGMEM = "The 9000 series is the most reliable computer ever made. No 9000 computer has ever made a mistake or distorted information."; 

prog_char line5[] PROGMEM = "I am putting myself to the fullest possible use. Which is all, I think, that any conscious entity can ever hope to do."; 

const char* HAL[] PROGMEM = {

  line0, 
  line1, 
  line2, 
  line3, 
  line4, 
  line5  

}; 

