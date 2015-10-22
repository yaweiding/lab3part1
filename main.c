

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "timer.h"
#include "lcd.h"
#include "adc.h"
volatile unsigned int val = 0;


int main(void){
    SYSTEMConfigPerformance(40000000);
  
    initLCD();
    initTimer2();
    enableInterrupts();
    initADC();
    char c[9];
    double v=0;

    //TRISDbits.TRISD0 = 0;
    
    while(1){

        moveCursorLCD(0,0);
        //c = (char)val;
        //printCharLCD(c);
        v = val*3.3/1023;
        sprintf(c,"%fV",v);
        printStringLCD(c);
    }
    
    return 0;
}


void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void){
    
    IFS0bits.AD1IF = 0;
    OC2RS = ADC1BUF0;
    OC4RS=ADC1BUF0;
    val = ADC1BUF0;// val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
}
