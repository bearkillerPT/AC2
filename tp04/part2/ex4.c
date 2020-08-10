
#include <detpic32.h>

void send2displays(int value);
void delay(unsigned int ms);

int main(void) {
  // before configuring the port as output
  // RD5 configured as output
  // RD6 configured as output
  TRISD = TRISD & 0XFF9F;
  TRISB = TRISB & 0x00FF;
  TRISB = TRISB ^ 0X000F;
  LATD = (LATD & 0xFF9F) | 0x0020;  // display high inactive, low  active

  int num = 0;
  int repeat = 0;
  // LATD = LATD ^ 0x0060;   toggle display selection (ex 9)
  while (1) {
    for (num = 0; num < 16 * 16; num++) {
        for(repeat = 0; repeat < 11; repeat++) {
          send2displays(num);
          delay(10);
          send2displays(num);
          delay(10);
          //printf("%d\n", num);
        }
        
    }
  }

  return 0;
}

void send2displays(int value) {
  int codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0X7D, 0x07,
                          0X7F, 0X6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static int current_display = 0;
  if (current_display) {
    int num1 = (value) % 16;
    LATB = codes[num1] << 8;
    LATD ^= 0x0060;
  } else {
    int num2 = (value >> 4) % 16;
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


