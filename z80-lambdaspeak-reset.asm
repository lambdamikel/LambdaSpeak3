org #8000
.start
LD BC,#FBEE
LD A,#FF 
OUT (C),A
.loop1
IN A,(C) 
CP 128
JP NZ,loop1
RET
