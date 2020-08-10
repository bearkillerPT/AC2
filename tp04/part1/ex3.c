#include <detpic32.h>

void delay(unsigned int ms);
int decodeChar(char c);
void putDisplay(char c);

int main(void){
    // before configuring the port as output
    TRISBbits.TRISB8 = 0; // RB8 configured as output
    TRISBbits.TRISB9 = 0; // RB9 configured as output
    TRISBbits.TRISB10 = 0; // RB10 configured as output
    TRISBbits.TRISB11 = 0; // RB11 configured as output
    TRISBbits.TRISB12 = 0; // RB12 configured as output
    TRISBbits.TRISB13 = 0; // RB13 configured as output
    TRISBbits.TRISB14 = 0; // RB14 configured as output
    TRISBbits.TRISB15 = 0; // RB15 configured as output
    TRISDbits.TRISD5 = 0; // RD5 configured as output
    TRISDbits.TRISD6 = 0; // RD6 configured as output
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
    putDisplay(0);
    char sel;
    while(1) {
        sel = getChar();
        putDisplay(sel);
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
        printf("%c -> %d\n", c, res);
        res = res << 8;
        LATB = LATB | res;
}


