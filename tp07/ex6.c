#include <detpic32.h>
void delay(unsigned int ms);
void send2displays(int value);
void configureAll();

volatile int v = 0;

void _int_(4) isr_T1(void){
    AD1CON1bits.ASAM = 1;
    IFS1bits.AD1IF = 0;
    IFS0bits.T1IF = 0;
    
} 
//Timer3 Interrupts
void _int_(12) isr_T3(void){
    send2displays(v);
    IFS0bits.T3IF = 0;
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

int main() {
    configureAll();
    
    while(1) {
        if(PORTBbits.RB0 == 0 && PORTBbits.RB1 == 1) {
            IEC0bits.T1IE = 0;
        }
        else{
            IEC0bits.T1IE = 1;
        }
    }
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

void configureAll() {
    //Timer1 4hz
    T1CONbits.TCKPS = 3; // 1:256 prescaler 
    PR1 = 19531; // Fout = 20MHz / (256 * (19531 + 1)) = 4 Hz
    TMR1 = 0; 
    T1CONbits.TON = 1;
    //Timer3 Interrupts
    IPC1bits.T1IP = 3;
    IEC0bits.T1IE = 1;

    //Timer3 100hz
    T3CONbits.TCKPS = 2; // 1:4 prescaler 
    PR3 = 49999; // Fout = 20MHz / (4 * (49999 + 1)) = 100 Hz
    TMR3 = 0; 
    T3CONbits.TON = 1; 
    //Timer2 Interrupts
    IPC3bits.T3IP = 2; 
    IEC0bits.T3IE = 1; 

    TRISB = TRISB & 0x00FF; //7 segments
    TRISBbits.TRISB4 = 1; //Potenciometro
    TRISBbits.TRISB0 = 1; // * switches
    TRISBbits.TRISB1 = 1; //            *
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
    IFS0bits.T1IF = 0;
    IFS0bits.T3IF = 0;
    IFS1bits.AD1IF = 0; 
    EnableInterrupts();
}
