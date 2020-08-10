#include <detpic32.h>
void configBRG();
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

void configBRG() {
    U1MODEbits.BRGH = 0; // /=16
    U1BRG = 10; 
    U1MODEbits.PDSEL = 0; //8bit no parity
    U1MODEbits.STSEL = 0; //1 stop bit
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
