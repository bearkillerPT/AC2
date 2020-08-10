#include <detpic32.h>
void configUART(int baudrate, char parity, int stopBits);
void putc(char byte2send);
void delay(unsigned int ms);
void putstr(char *str);
int main() {
    configBRG();
    while(1) {
        putstr("String de teste\n");
        delay(1000);
    }
}

void configUART(int baudrate, char parity, int stopBits) {
    U1MODEbits.BRGH = 0; // /=16
    if(!(baudrate >= 600 && baudrate <= 115200)) {
        baudrate = 115200;
    }
    U1BRG = ((20000000 + 8 * baudrate) / (16 * baudrate)) – 1;
    switch (parity)
    {
    case 'E':
        U1MODEbits.PDSEL = 1;
        break;
    case 'O':
        U1MODEbits.PDSEL = 2;
        break;
    default:
        U1MODEbits.PDSEL = 0;
    }
    if(!(stopBits == 1 || stopBits == 2)) stopBits = 1;
    U1MODEbits.STSEL = stopBits; //1 stop bit
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
}

void putc(char byte2send) {
    while(U1STAbits.UTXBF == 1);    // wait while UTXBF == 1
    U1TXREG = byte2send;            // Copy byte2send to the UxTXREG register
} 

void putstr(char *str) {
    while(*str != '\0') {
        putc(*str);
        str++;
    }
}

void delay(unsigned int ms) {
    int k = 20000;
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < k);
    }
}
