

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "timer.h"
#include "lcd.h"
#include "adc.h"
#include "pwm.h"
#include "switch.h"
#define leftmotor OC4RS
#define rightmotor OC2RS
#define PRESSED 0
#define RELEASED 1


volatile unsigned int val = 0;

typedef enum stateTypeEnum {
    forward, idle1, wait, backward, idle2, debouncePress, debounceRelease

} stateType;
volatile stateType state, nextState;
volatile int lcd = 1;


int main(void) {
    SYSTEMConfigPerformance(40000000);

    initLCD();
    initTimer2();
    initTimer3();
    initPWM();
    enableInterrupts();
    initADC();
    initSW1();
    char c[9];
    char adc[9];
    double v = 0;
    
    TRISDbits.TRISD0 = 0;
    LATDbits.LATD0 = 0;


    state = forward;

    while (1) {


        switch (state) {
            case forward:
                PINRD1 = 11; //0b1011;
                PINRB14 = 0;
                PINRC4 = 11; // 0b1011
                PINRF5 = 0;
                moveCursorLCD(0, 0);
                v = val * 3.3 / 1023;
                sprintf(c, "%fV", v);
                printStringLCD(c);

                moveCursorLCD(1, 0);
                sprintf(adc, "%d", val);
                printStringLCD(adc);
                
                
                break;
            
            case idle1:
                PINRD1 = 0; //0b1011;
                PINRB14 = 0;
                PINRC4 = 0; // 0b1011
                PINRF5 = 0;
                moveCursorLCD(0, 0);
                v = val * 3.3 / 1023;
                sprintf(c, "%fV", v);
                printStringLCD(c);

                moveCursorLCD(1, 0);
                sprintf(adc, "%d", val);
                printStringLCD(adc);
                
                break;
                
            case backward:
                PINRD1 = 0; //0b1011;
                PINRB14 = 11;
                PINRC4 = 0; // 0b1011
                PINRF5 = 11;
                moveCursorLCD(0, 0);
                v = val * 3.3 / 1023;
                sprintf(c, "%fV", v);
                printStringLCD(c);

                moveCursorLCD(1, 0);
                sprintf(adc, "%d", val);
                printStringLCD(adc);
                
                break;
            
            case idle2:
                PINRD1 = 0; //0b1011;
                PINRB14 = 0;
                PINRC4 = 0; // 0b1011
                PINRF5 = 0;

                moveCursorLCD(0, 0);
                v = val * 3.3 / 1023;
                sprintf(c, "%fV", v);
                printStringLCD(c);

                moveCursorLCD(1, 0);
                sprintf(adc, "%d", val);
                printStringLCD(adc);
                break;
                
            case debounceRelease:
                delayUs(5000);
                state = nextState;
                break;
        }

    }

    return 0;
}

void __ISR(_ADC_VECTOR, IPL6AUTO) _ADCInterrupt(void) {

    IFS0bits.AD1IF = 0;
    val = ADC1BUF0; // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;

    if (val > 460 && val < 562) { //full speed in the middle
        OC2RS = 1023;
        OC4RS = 1023;
    } else if (val <= 460) {
        OC2RS = 1023;
        OC4RS = 2 * val;
    } else if (val >= 562) {
        OC2RS = 2 * (1023 - val);
        OC4RS = 1023;
    }
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void) {
    IFS1bits.CNDIF = 0; // Put Interrupt flag down
    int dummysw1 = PORTDbits.RD6;
    
    if (SW1 == RELEASED && state == forward) {
        nextState = idle1;
        state = debounceRelease;
    }
    else if (SW1 == RELEASED && state == idle1) {
        nextState = backward;
        state = debounceRelease;
    }
    else if (SW1 == RELEASED && state == backward) {
        nextState = idle2;
        state = debounceRelease;
    }
    else if (SW1 == RELEASED && state == idle2) {
        nextState = forward;
        state = debounceRelease;
    }
}