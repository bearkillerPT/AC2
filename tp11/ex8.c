
#include <detpic32.h>
#define WRITE 0x02
#define RDSR 0x05
#define READ 0x03
#define WRSR 0x01
#define WRDI 0x04
#define WREN 0x06
#define EEPROM_CLOCK 500000 

void spi2_setClock(unsigned int clock_freq) {
    SPI2BRG = (20000000 + clock_freq) / (2 * clock_freq) - 1;
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

void eeprom_writeStatusCommand(char command) {
 while( eeprom_readStatus() & 0x01 ); // Wait while WIP is true
 // (write in progress)
 SPI2BUF = command;// Copy "command" value to SPI2BUF (TX FIFO)
 while(SPI2STATbits.SPIBUSY);   // Wait while SPI module is working (SPIBUSY set)
} 

void eeprom_writeData(int address, unsigned char value) {
    address = address & 0x01FF;
    int status = 1;
    while(status) status = eeprom_readStatus() & 0x0001;
    eeprom_writeStatusCommand(WREN); 
    SPI2BUF = WRITE | ((address & 0x100) >> 5);
    SPI2BUF = address & 0x00FF;
    SPI2BUF = value;
    while(SPI2STATbits.SPIBUSY);
} 

char eeprom_readData(int address) {
    volatile char trash;
    trash = SPI2BUF;
    SPI2STATbits.SPIROV = 0;
    address = address & 0x01FF;
    int status = 1;
    while(status) status = eeprom_readStatus() & 0x0001;
    SPI2BUF = READ | ((address & 0x100) >> 5);
    SPI2BUF = address & 0x00FF;
    SPI2BUF = 0x0000;
    while(SPI2STATbits.SPIBUSY);
    trash = SPI2BUF;
    trash = SPI2BUF;
    return SPI2BUF;
}

int main(void) {
    spi2_setClock(EEPROM_CLOCK);
    spi2_init();
    unsigned char val;
    int address;
    for(;;) {
        val = getChar();
        if(val == 'R' || val == 'r') {
            printStr("Read... Insira o address: ");
            address = readInt10();
            putChar('\n');
            printStr("Valor lido: ");
            printInt10(eeprom_readData(address));
            printStr("\nRead finished!\n");

        }
        else if(val == 'W' || val == 'w') {
            printStr("Write... Insira o address:\n");
            address = readInt10();
            printStr("Valor a ser escrito:\n");
            val = (unsigned char) readInt10();
            eeprom_writeData(address, val);
            printStr("Write finished!\n");
        }
    }
} 

