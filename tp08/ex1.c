#include <detpic32.h>
void configBRG();
int main() {

}

void configBRG() {
    U1MODEbits.BRGH = 0; // /=16
    U1BRG = 10; 
    U1MODEbits.PDSEL = 0; //8bit no parity
    U1MODEbits.STSEL = 0; //1 stop bit
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
}
