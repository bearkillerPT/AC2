#include <detpic32.h>
void configUART(int baudrate, char parity, int stopBits);
void putc(char byte2send);
void delay(unsigned int ms);
void putstr(char *str);
char getc();
void _int_(24) isr_uart1(void){
 putc(U1RXREG);
 IFS0bits.U1RXIF = 0;
} 
int main() {
    configUART(115200, 'N', 1);
    EnableInterrupts();
    while(1);
}

void configUART(int baudrate, char parity, int stopBits) {
    U1MODEbits.BRGH = 0; // /=16
    if(!(baudrate >= 600 && baudrate <= 115200)) {
        baudrate = 115200;
    }
    U1BRG = ((20000000 + 8 * baudrate) / (16 * baudrate)) - 1;
    switch (parity)
    {
    case 'E':
        U1MODEbits.PDSEL = 1;
        break;
    case 'O':
        U1MODEbits.PDSEL = 2;
        break;
    default:
        U1MODEbits.PDSEL = 0;
    }
    if(!(stopBits == 1 || stopBits == 2)) stopBits = 1;
    U1MODEbits.STSEL = stopBits; //1 stop bit
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    U1MODEbits.ON = 1;
    //interrupts
    U1STAbits.URXISEL = 0;
    IPC6bits.U1IP = 2;
    IEC0bits.U1RXIE = 1;
}

void putc(char byte2send) {
    while(U1STAbits.UTXBF == 1);    // wait while UTXBF == 1
    U1TXREG = byte2send;            // Copy byte2send to the UxTXREG register
} 

void putstr(char *str) {
    while(*str != '\0') {
        putc(*str);
        str++;
    }
}

void delay(unsigned int ms) {
    int k = 20000;
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < k);
    }
}

char getc(){
    char res;
    if(U1STAbits.OERR == 1) U1STAbits.OERR=0;
    while(U1STAbits.URXDA == 0);
    if(U1STAbits.FERR || U1STAbits.PERR) {
        res = U1RXREG;
        return 0;
    }
    else {
        return U1RXREG;
    }
} 
