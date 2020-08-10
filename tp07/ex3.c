#include <detpic32.h>

volatile int eachTwo = 0;

void _int_(12) isr_T3(void){
    if(eachTwo == 1) {
        eachTwo = 0;
        putChar('.');
    }
    else{
        eachTwo = 1;
    }
    IFS0bits.T3IF = 0;
} 

int main() {
    T3CONbits.TCKPS = 7; // 1:256 prescaler (i.e Fout_presc = 78125 Hz)
    PR3 = 39062; // Fout = 20MHz / (256 * (39062 + 1)) = 2 Hz
    TMR3 = 0; // Reset timer T2 count register
    T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the
    IPC3bits.T3IP = 3;
    IEC0bits.T3IE = 1;
    IFS0bits.T3IF = 0;
    EnableInterrupts();

    IFS0bits.T3IF = 0;
    while(1) {}
}
