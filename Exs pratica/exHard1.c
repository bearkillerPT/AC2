#include "detpic32.h"

#define fPBCLK 20000000

int main() {
    int bClk = 80, wClk = 80, refClk, numDisp;
    unsigned char last = 'b', current = 'w', finish;
    TRISE = TRISE & 0xFFFFFF00;
    TRISB = TRISB & 0xFFFF807F;
    TRISDbits.TRISD5 = 1;
    TRISDbits.TRISD5 = 0;
        while(1) {
            delay(1);
            refClk++; 
            current = inkey();
            if(current != 0 && current != last && (current == 'b'|| current == 'w')){char temp = last; last = current; current = last;} 
            if(refClk / 1000) {
                refClk = 0;
            }
            if(current == 'x') {
                finish = last;
            } 
            switch (current) {
                    case 'w':
                        wClk--;
                        break;
                    case 'b':
                        bClk--;
                        break;
                    default:
                        break;
            }
            if(wClk == 0) {
                finish = 'w';
                break;
            }
            else if(bClk == 0) {
                finish = 'b';
                break;
            }
            current == 'w'? numDisp = wClk % 10: numDisp = bClk % 10;
            send2displays(numDisp * 11);
            LATB = LATB & 0xFFFF807F;
            LATB = LATB ^ (numDisp << 7);
        }
    char* string;
    finish == 'b'? string = "Vitoria das Brancas!": string = "Vitoria das Pretas!";
    printStr(string);
}

void send2displays(int value) {
  int codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0X7D, 0x07,
                          0X7F, 0X6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static int current_display = 0;
  if (current_display) {
    int num1 = (value) % 10;
    LATB = codes[num1] << 8;
    LATD ^= 0x0060;
  } else {
    int num2 = (value / 10) % 10;
    LATB = codes[num2] << 8;
    LATD ^= 0x0060;
  }
  current_display = current_display ? 0 : 1;
}

void delay(unsigned int ms){
    int k = 20000;
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < k);
    }
}
