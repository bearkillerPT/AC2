#include <detpic32.h>
void delay(unsigned int ms);

int main() {

    TRISBbits.TRISB4 = 1; 
    TRISEbits.TRISE3 = 0; 
    AD1PCFGbits.PCFG4= 0; 
    AD1CON1bits.SSRC = 7; 
    AD1CON1bits.CLRASAM = 1; 
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 0; 
    AD1CHSbits.CH0SA = 4; 
    AD1CON1bits.ON = 1; 

    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1; 
    IFS1bits.AD1IF = 0; 
    LATEbits.LATE3 = 0;
    EnableInterrupts();
    AD1CON1bits.ASAM = 1; // Start convertion
    while(1) {}
}

void _int_(27) isr_adc() {
    LATEbits.LATE3 = 1;
    printInt(ADC1BUF0, 16 | 3 << 16);
    putChar('\n');
    IFS1bits.AD1IF = 0;
    AD1CON1bits.ASAM = 1; // Re-start convertion
    LATEbits.LATE3 = 0;
}

void delay(unsigned int ms){
    int k = 20000;
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < k);
    }
}

