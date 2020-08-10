#include <detpic32.h>
void configT3();
void configOC();
void configADC();
void adcConv();
void delay(unsigned int ms);

int main() {
    configT3();
    configOC();
    configADC();
    while(1) {
        adcConv();
    }
    return 0;

}

void adcConv() {
    resetCoreTimer();              // faz reset ao contador do core timer
    AD1CON1bits.ASAM = 1;
    while(IFS1bits.AD1IF == 0);
    
    // lê o valor atual to core timer
    printStr("\nConv. Time=");
    printInt10(readCoreTimer() * 50);        // O período do PBClk = 1/ 20MHz = 50ns
    printStr(" ns - ");

    printInt10(ADC1BUF0);
    IFS1bits.AD1IF = 0;              // esta linha tem que estar depois da leitura da ADC
    delay(500);
}

void configADC() {
    AD1PCFGbits.PCFG4= 0; 
    AD1CON1bits.SSRC = 7; 
    AD1CON1bits.CLRASAM = 1; 
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 7; 
    AD1CHSbits.CH0SA = 5; 
    AD1CON1bits.ON = 1; 
}

void configT3() {
    T3CONbits.TCKPS = 5; // 1:32 prescaler 
    PR3 = 6249; // Fout = 20000000 / (32 * (6249 + 1)) = 100 Hz
    TMR3 = 0; 
    T3CONbits.TON = 1; 
}

void configOC() {
    OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 1;// Use timer T3 as the time base for PWM generation
    OC1RS = 6249/4; // Ton constant
    OC1CONbits.ON = 1; 
    IFS1bits.AD1IF = 0; 
}

void delay(unsigned int ms){
    int k = 20000;
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < k);
    }
}

