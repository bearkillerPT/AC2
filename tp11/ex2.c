#include <detpic32.h>
#define WRITE 0x02
#define RDSR 0x05
#define READ 0x03
#define WRSR 0x01
#define WRDI 0x04
#define WREN 0x06
#define EEPROM_CLOCK 500000 

void spi2_setClock(unsigned int clock_freq) {
    SPI2BRG = (fPBCLK + fSCK) / (2 * fSCK) – 1;
} 

void spi2_init(void) {
    volatile char trash;
    SPI2CONbits.ON = 0; 
    SPI2CONbits.CKP = 0; 
    SPI2CONbits.CKE = 1;
    SPI2CONbits.SMP = 0;
    SPI2CONbits.MODE16 = 0;
    SPI2CONbits.MODE32 = 0;
    SPI2CONbits.ENHBUF = 1;
    SPI2CONbits.MSSEN = 1;
    SPI2CONbits.MSTEN = 1;
    while(SPI2STATbits.SPIRBE == 0) // while RX FIFO not empty read
    trash = SPI2BUF; // FIFO (discard data)
    SPI2STATbits.SPIROV = 0;// Clear overflow error flag
    SPI2CONbits.ON = 1; // Enable SPI2 module
} 

char eeprom_readStatus(void){
    volatile char trash;
    while(SPI2STATbits.SPIRBE == 0) // while RX FIFO not empty read
    trash = SPI2BUF;
    SPI2STATbits.SPIROV = 0;// Clear overflow error flag bit
    SPI2BUF = RDSR; // Send RDSR command
    SPI2BUF = 0; // Send anything so that EEPROM clocks data into SO
    while(SPI2STATbits.SPIBUSY); // wait while SPI module is working
    trash = SPI2BUF; // First char received is garbage (received while
    // sending command)
    return SPI2BUF; // Second received character is the STATUS value
    } 

