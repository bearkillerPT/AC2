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

typedef struct
{
    unsigned int count;
    unsigned int head;
    unsigned int tail;
} circBuffer;

int main(void) {
    spi2_setClock(EEPROM_CLOCK);
    spi2_init();
    char key = 0;
    int temp = 0;
    circBuffer buffer;
    circBuffer* buf = &buffer;
    buf->count = 0;
    buf->head = 0x02;
    buf->tail = 0x02;
    i2c1_init(TC74_CLK_FREQ);
    while(1) {
        if (key == 0) key = inkey();
        if(key == 'R'|| key == 'r') {
        	eeprom_writeData(0, 0);
        	eeprom_writeData(1, 0);
        	printStr("Log reseted!\n");
        	key = 0;
            buf->count = 0;
            buf->head = 0x02;
            buf->tail = 0x02;
        }
        else if(key =='L' || key == 'l'){
        	printStr("Log Started!\n");        
        	key = 0;
            for(;;) {
                if(key == 'R'|| key == 'r' || key == 'S' || key == 's') break;
                getTemperature(&temp);
                if(buf->count == 64){
                    eeprom_writeData(buf->head, (char) temp);
                    buf->head = (buf->head + 1);
                    buf->tail = (buf->tail + 1);
                    if(buf->head == 66) buf->head = 0x02;
                    if(buf->tail == 66) buf->tail = 0x02;
                    eeprom_writeData(1, buf->head - 2);
                } 
                else {
                    eeprom_writeData(0, buf->count + 1);
                    eeprom_writeData(1, buf->head - 2);
                    eeprom_writeData(buf->head, (char) temp);
                    buf->head = (buf->head + 1);
                    buf->count++;
                }
                key = delay(1000);
            }
        }
        else if(key == 'S' || key == 's') {
        	key = 0;
            int samples = eeprom_readData(0);
            printStr("eeprom[0] == buf->count: ");
            printInt10(samples == buf->count);
            putChar('\n');
            printStr("Numero de amostras: ");
            printInt10(samples);
            putChar('\n');
            int i;
            if(buf->tail < buf->head) {
                for(i = buf->tail; i < buf->head; i++) {
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
            else {
               for(i = buf->tail; i < buf->count - 1; i++) {
                if(i % 4 == 0) {
                    printStr("Sample minuto: ");
                    printInt10(i/4);
                    printStr(":\n");
                }
                printStr("  ");
                printInt10(eeprom_readData(0x02 + i));
                printStr(", \n");
                } 
                for(i = 0; i < buf->head; i++) {
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
  return key;
}

