#include <detpic32.h>

int main() {
    T3CONbits.TCKPS = 7; // 1:256 prescaler (i.e Fout_presc = 78125 Hz)
    PR3 = 39062; // Fout = 20MHz / (256 * (39062 + 1)) = 2 Hz
    TMR3 = 0; // Reset timer T2 count register
    T3CONbits.TON = 1; // Enable timer T2 (must be the last command of the
    IFS0bits.T3IF = 0;
    while(1) {
        if(IFS0bits.T3IF == 1) {
            putChar('.');
            IFS0bits.T3IF = 0;
        }
    }
}
