#include <detpic32.h>
#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE – 1)

void comDrv_flushRx(void);
void comDrv_flushTx(void);
void comDrv_putc(char ch);
void comDrv_puts(char* str);
void comDrv_config(int baudrate, char parity, int stopBits);


typedef struct {
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer; 

volatile circularBuffer txb; 
volatile circularBuffer rxb;

void _int_(24) isr_uart1(void) {
    if(IFS0bits.U1TXIF == 1) {
        if(txb.count > 0) {
            U1TXREG = txb.data[head];
            txb.head = (txb.head + 1) & INDEX_MASK;
            txb.count--;
        }
        else {
            DisableUart1TxInterrupt();
        }
        IFS0bits.U1TXIF = 0;
    }
    
} 

void comDrv_flushRx(void) {
    rxb.head = 0;
    rxb.tail = 0;
    rxb.count = 0;
}

void comDrv_flushTx(void) {
    rxb.head = 0;
    rxb.tail = 0;
    rxb.count = 0;     
}

void comDrv_putc(char ch) {
    while(txb.count == BUF_SIZE); 
    txb.data[txb.tail] = ch; 
    txb.tail = (txb.tail + 1) & INDEX_MASK; 
    DisableUart1TxInterrupt(); 
    count++;
    EnableUart1TxInterrupt(); 
} 

void comDrv_puts(char* str) {
    while(*str != '\0') {
        comDrv_putc(*str);
        str++;
    }
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
}


