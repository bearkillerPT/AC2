#include <detpic32.h>
void delay(unsigned int ms);
void send2displays(int value);

volatile int v = 0;

int main() {

    TRISB = TRISB & 0x00FF;
    TRISBbits.TRISB4 = 1; 
    TRISD = TRISD & 0XFF9F;
    LATD = (LATD & 0xFF9F) | 0x0020;
    AD1PCFGbits.PCFG4= 0; 
    AD1CON1bits.SSRC = 7; 
    AD1CON1bits.CLRASAM = 1; 
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 7; 
    AD1CHSbits.CH0SA = 4; 
    AD1CON1bits.ON = 1; 

    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1; 
    IFS1bits.AD1IF = 0; 
    LATEbits.LATE3 = 0;
    EnableInterrupts();

    int i = 0;
    while(1) {
        delay(10);
        if(i++ == 25) {
            AD1CON1bits.ASAM = 1; // Start convertion
            i = 0;
        }
        send2displays(v);
    }
}

void _int_(27) isr_adc() {
    v = 0;
    int* p = (int *)(&ADC1BUF0);
    for(; p <= (int *)(&ADC1BUF7); p+=4 )
    {
        v += (*p * 33 + 511) / 1023;
    } 
    v /= 8;
    IFS1bits.AD1IF = 0;
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
