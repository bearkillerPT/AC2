#include <detpic32.h>

void delay(unsigned int ms);
int decodeChar(char c);
void putDisplay(char c);

int main(void){
    // before configuring the port as output
    TRISDbits.TRISD5 = 0; // RD5 configured as output
    TRISDbits.TRISD6 = 0; // RD6 configured as output
    TRISB = TRISD & 0x00FF;
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    putDisplay(0);
    char leters[7] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    int i = 0;
    while(1) {
        for(i = 0; i < 7; i++) {
            putDisplay(leters[i]);
            delay(500);
        }
        LATDbits.LATD5 = !LATDbits.LATD5;
        LATDbits.LATD6 = !LATDbits.LATD6;
       
    }
     return 0;
}

void delay(unsigned int ms){
    int k = 20000;
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < k);
    }
}
int decodeChar(char c) {
    switch (c)
    {
        case 'a': case 'A':
            return 0b01110111;
        case 'b': case 'B':
            return 0b01111111;
        case 'c': case 'C':
            return 0b00111001;
        case 'd': case 'D':
            return 0b00111111;
        case 'e': case 'E':
            return 0b01111001;
        case 'f': case 'F':
            return 0b01110001;
        case 'g': case 'G':
            return 0b01111101;
        case '.':
            return 0b10000000;
        default:
            return 0b00111111;
    }
}

void putDisplay(char c) {
        int res;
        LATB = LATB & 0x00FF;
        res = decodeChar(c);
        //printf("%c -> %d\n", c, res);
        res = res << 8;
        LATB = LATB | res;
}


