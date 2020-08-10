#include <detpic32.h>
#include "i2c.h"
#include "eeprom.h"
unsigned char delay(unsigned int ms);

int getTemperature(int *temperature) {
    int ack;
    i2c1_start();
    ack = i2c1_send(ADDR_WR);
    ack += i2c1_send(RTR);
    i2c1_start();
    ack += i2c1_send(ADDR_RD);
    if(ack != 0) {
        i2c1_stop();
        printStr("Ack error exiting...");
        return 1;
    }
    *temperature = i2c1_receive(I2C_NACK);
    i2c1_stop();
// Send Stop event
    return ack;
}

int main(void) {
    spi2_setClock(EEPROM_CLOCK);
    spi2_init();
    int addr = 0x02;
    int samples = 0;
    char key = 0;
    int temp = 0;
    i2c1_init(TC74_CLK_FREQ);
    while(1) {
        if (key == 0) key = inkey();
        if(key == 'R'|| key == 'r') {
        	eeprom_writeData(0, 0);
        	printStr("Log reseted!\n");
        	key = 0;
        }
        else if(key =='L' || key == 'l'){
        	printStr("Log Started!\n");        
        	key = 0;
            for(samples = 0; samples < 64; samples++) {
                if(key == 'R'|| key == 'r' || key == 'S' || key == 's') break;
                getTemperature(&temp);
                eeprom_writeData(0, (char) samples + 1);
                eeprom_writeData(addr, (char) temp);
                addr++;
                key = delay(15000);
            }
        }
        else if(key == 'S' || key == 's') {
        	key = 0;
            samples = eeprom_readData(0);
            printStr("Numero de amostras: ");
            printInt10(samples);
            putChar('\n');
            int i;
            for(i = 0; i < samples; i++) {
                if(i % 4 == 0) {
                    printStr("Sample minuto: ");
                    printInt10(i/4);
                    printStr(":\n");
                }
                printStr("  ");
                printInt10(eeprom_readData(0x02 + i));
                printStr(", \n");
            }
        }
    }
} 

unsigned char delay(unsigned int ms) {
  int k = PBCLK / 1000;
  unsigned char key = 0;
  
  for (; ms > 0; ms--) {
    resetCoreTimer();
    while (readCoreTimer() < k) {
    	 key = inkey();
    	 if (key != 0) return key;
    }
  }
}

