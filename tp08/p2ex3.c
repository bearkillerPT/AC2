#include <detpic32.h>
void delay(unsigned int ms);
void send2displays(int value);
void setPWM(unsigned int dutyCycle);
void configUART(int baudrate, char parity, int stopBits);
void putc(char byte2send);
void configureAll();

volatile int v = 0;
volatile int mod100T3 = 0;
volatile int vMin = 33;
volatile int vMax = 0;

void _int_(4) isr_T1(void){
    AD1CON1bits.ASAM = 1;
    IFS0bits.T1IF = 0;
} 
//Timer3 Interrupts
void _int_(12) isr_T3(void){
    send2displays(v);
    mod100T3++;
    if(mod100T3 == 100) {
        putc(v/10 + 48);
        putc(v%10 + 48);
        putc('\n');
        mod100T3 = 0;
    }
    IFS0bits.T3IF = 0;
} 
void _int_(24) isr_uart1(void) {
    if(IFS0bits.U1EIF == 1) {
        if(U1STAbits.OERR == 1) U1STAbits.OERR=0;
        char res = U1RXREG;
        IFS0bits.U1EIF = 0;
    }
    else{
        if(U1RXREG == 'l' || U1RXREG == 'L') {
            putc(vMin/10+48);
            putc(vMin%10+48);
            putc('-');
            putc(vMax/10+48);
            putc(vMax%10+48);
            putc('\n');
        }
    }
    
    IFS0bits.U1RXIF = 0;
} 
void _int_(27) isr_adc() {
    v = 0;
    int* p = (int *)(&ADC1BUF0);
    for(; p <= (int *)(&ADC1BUF7); p+=4 )
    {
        v += (*p * 33 + 511) / 1023;
    } 
    v /= 8;
    if(v > vMax) vMax = v;
    if(v < vMin) vMin = v;
    IFS1bits.AD1IF = 0;
}

int main() {
    configureAll();
    IFS0bits.T1IF = 0;
    IFS0bits.T3IF = 0;
    IFS1bits.AD1IF = 0;
    int switches;
    while(1) {
        switches = PORTB & 0x0003;
        switch (switches)
        {
        case 0://00
            IEC0bits.T1IE = 1;
            setPWM(0);
            break;
        case 2: //01 (bits por ordem inversa)
            IEC0bits.T1IE = 0;
            setPWM(100);
            break;
        default:
            IEC0bits.T1IE = 1;
            setPWM(3*v);
            break;
        }
    }
}

void setPWM(unsigned int dutyCycle) {
    if(dutyCycle<=100 && dutyCycle>=0) OC1RS = PR3*dutyCycle/100;
}

void send2displays(int value) {
  int codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0X7D, 0x07,
                          0X7F, 0X6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static int current_display = 0;
  if (current_display) {
    int num1 = (value) % 10;
    LATB = codes[num1] << 8;
    LATD ^= 0x0060;
  } else {
    int num2 = (value / 10) % 10;
    LATB = codes[num2] << 8;
    LATD ^= 0x0060;
  }
  current_display = current_display ? 0 : 1;
}

void delay(unsigned int ms){
    int k = 20000;
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < k);
    }
}

void configureAll() {
    //Timer1 4hz
    T1CONbits.TCKPS = 3; // 1:256 prescaler 
    PR1 = 19531; // Fout = 20MHz / (256 * (19531 + 1)) = 4 Hz
    TMR1 = 0; 
    T1CONbits.TON = 1;
    //Timer3 Interrupts
    IPC1bits.T1IP = 3;
    IEC0bits.T1IE = 1;

    //Timer3 100hz
    T3CONbits.TCKPS = 2; // 1:4 prescaler 
    PR3 = 49999; // Fout = 20MHz / (4 * (49999 + 1)) = 100 Hz
    TMR3 = 0; 
    T3CONbits.TON = 1; 
    //Timer2 Interrupts
    IPC3bits.T3IP = 2; 
    IEC0bits.T3IE = 1; 

    OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 1;// Use timer T3 as the time base for PWM generation
    OC1RS = 0; // Ton constant
    OC1CONbits.ON = 1; 
    IFS1bits.AD1IF = 0; 

    TRISB = TRISB & 0x00FF;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB4 = 1; 
    TRISD = TRISD & 0XFF9F;
    LATD = (LATD & 0xFF9F) | 0x0020;
    AD1PCFGbits.PCFG4= 0; 
    AD1CON1bits.SSRC = 7; 
    AD1CON1bits.CLRASAM = 1; 
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 7; 
    AD1CHSbits.CH0SA = 4; 
    AD1CON1bits.ON = 1; 

    IPC6bits.AD1IP = 2;
    IEC1bits.AD1IE = 1;  
    LATEbits.LATE3 = 0;
    configUART(115200, 'N', 1);
    EnableInterrupts();
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