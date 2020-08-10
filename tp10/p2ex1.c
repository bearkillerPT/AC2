#include <detpic32.h>
#include "i2c.h"
void delay(unsigned int ms);

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
    int temp = 0;
    i2c1_init(TC74_CLK_FREQ);
    while(1) {
        getTemperature(&temp);
        printStr("Read: ");
        printInt10(temp);
        putChar('\n');
        delay(250);
    }
} 

void delay(unsigned int ms) {
  int k = PBCLK / 1000;
  for (; ms > 0; ms--) {
    resetCoreTimer();
    while (readCoreTimer() < k)
      ;
  }
}
