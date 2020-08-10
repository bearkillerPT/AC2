#include <detpic32.h>

//Timer1 Interrupts
void _int_(4) isr_T1(void){
    putChar('1');
    IFS0bits.T1IF = 0;
} 
//Timer3 Interrupts
void _int_(12) isr_T3(void){
    putChar('3');
    IFS0bits.T3IF = 0;
} 

int main() {
    //Timer1 2hz
    T1CONbits.TCKPS = 3; // 1:256 prescaler (i.e Fout_presc = 78125 Hz)
    PR3 = 39062; // Fout = 20MHz / (256 * (39062 + 1)) = 2 Hz
    TMR3 = 0; 
    T1CONbits.TON = 1;
    //Timer3 Interrupts
    IPC1bits.T1IP = 3;
    IEC0bits.T1IE = 1;
    IFS0bits.T1IF = 0;

    //Timer3 10hz
    T3CONbits.TCKPS = 5; // 1:32 prescaler (i.e. fout_presc = 625 KHz)
    //For 20 hz just change TCKP5 to 4!
    PR3 = 62499; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR3 = 0; 
    T3CONbits.TON = 1; 
    //Timer2 Interrupts
    IPC3bits.T3IP = 2; 
    IEC0bits.T3IE = 1; 
    IFS0bits.T3IF = 0;
    EnableInterrupts();

    IFS0bits.T3IF = 0;
    while(1) {}
}
