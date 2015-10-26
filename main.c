

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
#define press 0
#define release 1


volatile unsigned int val = 0;
typedef enum stateTypeEnum {
    forward, idle1, wait1, backward, idle2, wait2
 
} stateType;
volatile stateType state;
volatile int lcd=1;
// input1 left front input 2 left backward input4 right front input3 right backward
int main(void){
    SYSTEMConfigPerformance(40000000);
  
    initLCD();
    initTimer2();
    initTimer3();
    initSW1();
    enableInterrupts();
    initADC();
    initPWM();
    char c[9];
    double v=0;
    state=idle1;

    while(1){      if(lcd){
                    moveCursorLCD(0,0);
                    v = val*3.3/1023;
                    sprintf(c,"%fV",v);
                    printStringLCD(c);
                    lcd=0;
                       }
        switch(state){
               
                case forward:
                   PINRD3=11;// mapping the left front
                   PINRD11=0; 
                   
                   PINRD1=11;// mapping the right front
                   PINRD5=0; 
                   state=wait1;
                   break;
               
                 case idle1:
                    leftmotor=0;
                    rightmotor=0;
                    state=backward;
                    break;
               
               
                case backward:
                   PINRD3=0;// mapping the left backward
                   PINRD11=11; 
                   
                   PINRD1=0;// mapping the right backward
                   PINRD5=11; 
                   state=wait1;
                   break;
                   
                case idle2:
                  leftmotor=0;
                  rightmotor=0;
                  state=forward;
                  break;
            case wait1:
                if(val>511)
                {
                    leftmotor=1023;
                    rightmotor=1023-2*val;
                    
                }
                else if(val<511)
                {
                    leftmotor=2*val;
                    rightmotor=1023;
                   
                }
                else 
                {
                    leftmotor=1023;
                    rightmotor=1023;
                }
                break;
                
                
            
            default:
                state=forward;
                    
          }
    
       
    }
    
    return 0;
}


void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void){
    
    IFS0bits.AD1IF = 0;
    lcd=1;
    val = ADC1BUF0;
        
}
void __ISR(_CHANGE_NOTICE_VECTOR, IPL3SRS) _CNInterrupt(void){
    IFS1bits.CNDIF = 0;         // Put Interrupt flag down
    if(state==forward && SW1==0){
      // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
       state=idle1;
    }
    if(state==backward && SW1==0){
     // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
       state=idle2;
    }
 
    
}


/*

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
*/