# The Making of LambdaSpeak for the Amstrad CPC 
## From Humble Beginnings as an Arduino Project to a Multi-Purpose CPC Expansion Board

*This is an article that was invited by the editor of the "Amstrad CPC
ACUC (Amstrad CPC User Club Magazine)", Dr.CPC. Unfortunately, this 
fanzine no longer seems to exist, and the article never got published,
so I thought I put it up here. Enjoy!* 

My [(LambdaMikel's)](https://hackaday.io/projects/hacker/138722)
fascination with speech synthesis started in the early 80s, and was
likely ignited and then nourished by movies such as ["Colossus - The
Forbin Project"](https://youtu.be/kyOEwiQhzMI),
["2001"](https://youtu.be/oR_e9y-bka0), and
["Wargames"](https://youtu.be/TQUsLAAZuhU).

When I got my [Schneider CPC 464 with CTM 640 Color
Monitor](articlepics/mycpc.jpg) for my confirmation in April 1985, the
first hardware extension that I acquired in July 1985 was the freshly
released **DK'tronics speech synthesizer**, containing the infamous
**SP0256-AL2 speech chip** used in so many speech synths of that
time. Soon it gave the "Eliza" program from [David H. Ahl's "BASIC
Computer Games"](https://en.wikipedia.org/wiki/BASIC_Computer_Games) a
voice, and it could even speak German (albeit with strong American
accent)!

![DK'tronics Speech Synthesizer](./articlepics/dktronics.png)

Whereas the DK'tronics was certainly a cherished item, I was
nevertheless a bit disappointed by the SP0256-AL2's limited
capabilities. The speech quality was lacking and too monotone and
robotic for my taste, and, of course, no match for HAL 9000... but
even in comparison to the computer voice in "Wargames" (I later
learned that the Wargames computer speech synth wasn't real
either). Also, it certainly didn't help the SP0256-AL2's performance
that DK'tronics made a serious design simplification - the SP0
requires a proper 3.12 MHz clock and hence extra crystal for proper
operation, but to save some pennies, DK'tronics decided to omit the
crystal and used the CPC's 4MHz clock instead. This resulted in a
speech pitch and allophone speak rate that was 22% too high, severely
damaging voice clarity. Later, Amstrad would release the SSA-1 using
the same chip, but with the extra crystal. In comparison, the SSA-1
sounds much better than the DK’tronics indeed. I wondered if maybe one
day I could design a more natural sounding voice synthesizer for the
CPC that sounded as good as HAL 9000! One day...

Fast forward 30 years (in 2016), I started tinkering with Arduinos,
and came across the Emic-2 speech synthesizer module in Palo Alto's
[Fry's Electronics](https://en.wikipedia.org/wiki/Fry%27s_Electronics)
store. It's quality of speech blew me away - finally, natural sounding
speech from a compact, affordable little unit! I realized that this
unit was the key towards realizing my dream from the 80s. I learned
that the Emic-2 was running the legendary [DECtalk speech synthesizer
software](https://en.wikipedia.org/wiki/DECtalk), and that it could
even sing, just like HAL 9000! 

And, sure enough, in May 2017, I released the first version of
LambdaSpeak using the Emic-2. Emic-2 was controlled over a serial
interface from an Atmega 328p (Arduino Uno R3) microcontroller chip
(as so-called "shrimped" Arduino design). In addition, a bunch of glue
logic was used, featuring two GAL 16V8 for address decoding, some
combinational logic (74LSxxx gates) for databus handling, etc. Later
versions also included a loudspeaker and were designed to not block
the display of my beloved DDI-3 HxC disc emulator interface connected
to my CPC 464. I was just having fun.

![LambdaSpeak 1 using the Emic-2](articlepics/emic4cpc.jpg)
![LambdaSpeak 1.5](articlepics/ls1.jpg)

However, Emic-2 was (too) pricey, my first designs were too clunky,
and so I was looking for an alternative. Realizing that the Emic-2 was
centered around the **Epson S1V30120 DSP**, I found a module which was
only half the price, and which became the heart of all future
LambdaSpeak designs: the Text2Speech "Click!" board from
MikroElektronika.

![Text2Speech Module](articlepics/epson.png)

In order to use it, I had to switch to a more powerful
microcontroller, for which I chose the Atmega 644 with 64 KB of
program memory. The 32 Kbs found in the ATmega 328p were no longer
sufficient to store the S1V30120 firmware image which includes the
DECtalk software (the Emic-2 was hosting this firmware in it's own
microcontroller).

Whereas the original focus of LambdaSpeak was to provide natural
sounding speech, it dawned me that it would be nice if it could also
*emulate* the original DK'tronics and SSA-1 speech synthesizers so
that existing speech synth software (especially games with speech)
would run with it. I hence worked out a mapping from SP0256-AL2
"allophones" to DECtalk phonemes and learned how to emulate the SP0
chip signals so that existing speech software would detect an SSA-1 or
DK'tronics speech synth when in fact there was a LambdaSpeak.
Finally, in February 2018, the SP0 emulation was working well enough
so that existing SSA-1 and DK'tronics speech software would run with
LambdaSpeak. LambdaSpeak 1.5 was born, and I started to sell a few
units to like-minded CPC enthusiasts.

These first LambdaSpeak customers started to make feature requests!
For example, Zhulien from the CPC Wiki EU Forum suggested the "Amdrum
emulation mode". This mode turned LambdaSpeak into a high-quality PCM
8bit sample player, and the original Amdrum drum computer software
sounds as good with LambdaSpeak as with the original Amdrum hardware
(if not better). Later, TFM ported Prodatron's Digitracker (an 8Bit
PCM Sound Tracker meant for the DigiBlaster) to use the Amdrum mode as
well.

At some point, Bryce from the CPC Wiki EU Forum also provided some
guidance and ideas; for example, he reviewed one of my earlier designs
and suggested some improvements. He also recommended to replace the
two GAL’s and remaining 72LSxxx glue logic by a single Xilinx CPLD
(Complex Programmable Logic Device, similar to an FPGA) to reduce the
chip count and hence costs. Thanks to Bryce, I started to learn
Verilog and familiarized myself with CPLDs. A CPLD was used for
LambdaSpeak III (see below) and for some of my future projects (e.g.,
[Speak&SID for the CPC](https://github.com/lambdamikel/Speak-SID))

[TFM of FutureSoft](https://www.cpcwiki.eu/index.php/TFM) joined the
LambdaSpeak development team in early 2018 - whereas early versions of
LambdaSpeak only came with a set of primitive BASIC demo programs and
the original SSA-1 and DK'tronics software, TFM now started to develop
a full-fledged and feature-complete expansion ROM containing dozens of
RSX commands, truly unleashing its full potential! After a number of
revisions, LambdaSpeak 1.95 was the first feature-complete and stable
version of which about 20 units were sold in 2018, and which now came
with TFM’s sophisticated RSX software. Here is a picture of
LambdaSpeak 1.95 with TFM's RSX ROM hosted by ToTO's Xmem.

![LambdaSpeak 1.95](articlepics/ls195.jpg)

Whilst selling LambdaSpeak 1.95, I completely redesigned the hardware
and replaced the combinational logic with a single powerful Xilinx
XC9572 PLLC84 CPLD. More and more features were added, eventually
culminating in the release of LambdaSpeak III in March 2019. A
highlight of LambdaSpeak III was a socket for the original SP0256-AL2,
as a second speech chip option! With the original SSA-1 / DK'tronics
speech chip on board, LambdaSpeak III is also a modern
re-implementation of these classic speech synths, not only an
emulation. With a SP0256-AL2 on board it sounds exactly like the
originals, and it also has the proper 3.12 MHz crystal. However, the
SP0256-AL2 is optional and can be difficult to source, so you will
still get the DECtalk-based SP0 emulation without it.

![LambdaSpeak 3](articlepics/ls3.jpg)

Eventually, the final versions of LambdaSpeak III featured:
- natural sounding DECtalk speech synthesis that puts HAL 9000 to shame, supporting English and Castilian Spanish; 
- the SP0256-AL2 emulation, 
- an optional SP0256-AL2 as a second speech chip option - if present, LambdaSpeak III is also a SSA-1 and DK'tronics re-implementation that sounds like the originals; 
- the Amdrum PCM sample playing mode; 
- 128 KBs of (SPI) EEPROM providing for non-volatile data storage - it's main purpose is to act as PCM sample memory for the 
- 8bit hiqh-quality PCM sample player mode, realizing 4-channel PCM sound of unheard quality on the CPC; this mode differs from the Amdrum PCM mode in that the CPC does not have to stream the PCM sample data - instead, the data is coming from the EEPROM and it can be played back autonomously by LambdaSpeak without CPC involvement, hence greatly reducing CPU load; 
- a general purpose serial interface that can be used to control, for example, a serial MP3 player or a MIDI interface breakout board; and finally, 
- a battery backed-up real-time clock with temperature sensor (a DS3231 via I2C protocol).

Unfortunately, LambdaSpeak III turned out to be too pricey for broad
customer acceptance (with SP0-256AL2 chip, it was in the 120 € range!),
and sales had been slow. Given both TFM's as well as LambdaMikel's
huge time and financial investment in developing it, TFM suggested in
November 2020 to improve the situation by bringing "LambdaSpeak to the
masses, not the classes" - the idea to develop a much more affordable
"cost-down" version was born. Hence, the SP0256-AL2 option as well as
the EEPROM and extended PCM sample playing modes were dropped, and the
MP3 and RTC modules were added by default as they were received more
favorably by customers. LambdaSpeak "FutureSoft Edition", or LFS for
short, was born!

The slightly reduced capabilities of LFS made a massively cost-reduced
hardware possible - the expensive (and increasingly difficult to
source) Xilinx CPLD was replaced by a single 22V10 GAL for address
decoding, accompanied by the same ATmega 644 microcontroller, and a
74LS374 flip flop in tandem with a 74LS244 line driver for CPC databus
management. LambdaMikel designed the LFS hardware over the
Thanksgiving weekend 2020, and delivered a first prototype to TFM in
December 2020.

![LambdaSpeak FS Front](articlepics/lsfs1.jpg)

![LambdaSpeak FS Back](articlepics/lsfs2.jpg)

TFM started refactoring the LS III ROM software for LFS. Finally, the
first batch of 10 LFS was shipped to TFM for distribution in January
2021.  Eventually, TFM took on production of further units, continued
to develop the ROM software, provided customer support, shipping, and
spread the word in the CPC Community.

![LambdaSpeak FS First Batch](articlepics/lsfs3.jpg)

At the time of this writing, TFM is the sole point of contact for all
LFS-related issues, including sales.  So, join the future of speech
synthesis, and give your CPC a voice – get a LambdaSpeak FS from TFM!
And be sure to read TFM’s LambdaSpeak RSX ROM software description in
this issue as well. You can find a lot of information about
LambdaSpeak on GitHub – everything is OpenSource, including the
hardware & software, demo programs and videos, etc.

- [https://github.com/lambdamikel/LambdaSpeak-FS](https://github.com/lambdamikel/LambdaSpeak-FS)
- [https://github.com/lambdamikel/LambdaSpeak3](https://github.com/lambdamikel/LambdaSpeak3)

*Sincerely yours,* 

**LambdaMikel, June 20th, 2022, Palo Alto, CA, USA** 




