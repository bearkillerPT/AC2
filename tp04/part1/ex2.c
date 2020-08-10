#include <detpic32.h>

void delay(unsigned int ms);

int main(void){
    // before configuring the port as output
    TRISEbits.TRISE0 = 0; // RE0 configured as output
    TRISEbits.TRISE1 = 0; // RE1 configured as output
    TRISEbits.TRISE2 = 0; // RE2 configured as output
    TRISEbits.TRISE3 = 0; // RE3 configured as output
    LATEbits.LATE0 = 0; // The initial value should be set
    LATEbits.LATE1 = 0; // The initial value should be set
    LATEbits.LATE2 = 0; // The initial value should be set
    LATEbits.LATE3 = 0; // The initial value should be set
    int i = 0;  
    while(1){
        delay(250); // half period = 0.5s
        LATE = LATE & 0xFFF0;
        LATE = LATE | i;
        i++;
        if(i > 15)
            i = 0;
        
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
