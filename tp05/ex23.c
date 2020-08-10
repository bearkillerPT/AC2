#include <detpic32.h>

void delay(unsigned int ms);
int main() {
    TRISBbits.TRISB4 = 1; // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0; // RBx configured as analog input (AN4)
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
                          // mode an internal counter ends sampling and
                          // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
                          // interrupt is generated. At the same time,
                          // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 3; // Interrupt is generated after XX samples
    // (replace XX by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
 // This must the last command of the A/D
 // configuration sequence 
    
    while(1) {
        AD1CON1bits.ASAM = 1;
        while( IFS1bits.AD1IF == 0 );
        IFS1bits.AD1IF = 0;
        int *p = (int *)(&ADC1BUF0);
        for(; p <= (int *)(&ADC1BUFF); p+=4 )
        {
            printInt(*p, 10 | 4 << 16);
            printf(" ");
        } 
        printf("\n");
        delay(500);
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
