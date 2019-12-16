                                         /* SPI EEPROM 25LC256 Library */
#include <avr/io.h>

// redefine this for ATMEGA 644p !


#define SPI_SS                     PB3
#define SPI_SS_PORT                PORTB
#define SPI_SS_PIN                 PINB
#define SPI_SS_DDR                 DDRB

#define SPI_MOSI                     PB5
#define SPI_MOSI_PORT                PORTB
#define SPI_MOSI_PIN                 PINB
#define SPI_MOSI_DDR                 DDRB

#define SPI_MISO                     PB6
#define SPI_MISO_PORT                PORTB
#define SPI_MISO_PIN                 PINB
#define SPI_MISO_DDR                 DDRB

#define SPI_SCK                     PB7
#define SPI_SCK_PORT                PORTB
#define SPI_SCK_PIN                 PINB
#define SPI_SCK_DDR                 DDRB



                                 /* Which pin selects EEPROM as slave? */
#define SLAVE_SELECT    SPI_SS_PORT &= ~(1 << SPI_SS)
#define SLAVE_DESELECT  SPI_SS_PORT |= (1 << SPI_SS)

// Instruction Set -- from data sheet
#define EEPROM_READ      0b00000011                     /* read memory */
#define EEPROM_WRITE     0b00000010                 /* write to memory */
#define EEPROM_CE        0b11000111 // chip erase
#define EEPROM_PE        0b01000010 // page erase 

#define EEPROM_WRDI      0b00000100                   /* write disable */
#define EEPROM_WREN      0b00000110                    /* write enable */

#define EEPROM_RDSR      0b00000101            /* read status register */
#define EEPROM_WRSR      0b00000001           /* write status register */

#define EEPROM_RDID      0b10101011 // get EEPROM ID 

// EEPROM Status Register Bits -- from data sheet
// Use these to parse status register
#define EEPROM_WRITE_IN_PROGRESS    0
#define EEPROM_WRITE_ENABLE_LATCH   1
#define EEPROM_BLOCK_PROTECT_0      2
#define EEPROM_BLOCK_PROTECT_1      3

#define EEPROM_BYTES_PER_PAGE       256
#define EEPROM_BYTES_MAX            0x1FFFF

// Functions

                  /* Init SPI to run EEPROM with phase, polarity = 0,0 */
void EEPROM_initSPI(void);

                  /* Generic.  Just loads up HW SPI register and waits */
void EEPROM_SPI_tradeByte(uint8_t byte);

                     /* splits 32-bit address into 3 bytes, sends both */
void EEPROM_send24BitAddress(uint32_t address);

                                   /* reads the EEPROM status register */
uint8_t EEPROM_readStatus(void);

                                   /* helper: sets EEPROM write enable */
void EEPROM_writeEnable(void);

                           /* gets a byte from a given memory location */
uint8_t EEPROM_readByte(uint32_t address);

                        /* gets two bytes from a given memory location */
uint16_t EEPROM_readWord(uint32_t address);

                           /* writes a byte to a given memory location */
void EEPROM_writeByte(uint32_t address, uint8_t byte);

                          /* gets two bytes to a given memory location */
void EEPROM_writeWord(uint32_t address, uint16_t word);

                                  /* sets every byte in memory to zero */
void EEPROM_clearAll(void);
