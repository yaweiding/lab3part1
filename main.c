

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
volatile int lcd = 1;
//// input1 left front input 2 left backward input4 right front input3 right backward
//int main(void){
//    SYSTEMConfigPerformance(40000000);
//  
//    initLCD();
//    initTimer2();
//    initTimer3();
//    initSW1();
//    enableInterrupts();
//    initADC();
//    initPWM();
//    char c[9];
//    double v=0;
//    state=idle1;
//
//    while(1){      if(lcd){
//                    moveCursorLCD(0,0);
//                    v = val*3.3/1023;
//                    sprintf(c,"%fV",v);
//                    printStringLCD(c);
//                    lcd=0;
//                       }
//        switch(state){
//               
//                case forward:
//                   PINRC13=11;// mapping the left front
//                   PINRF5=0; 
//                   
//                   PINRD1=11;// mapping the right front
//                   PINRB14=0; 
//                   state=wait1;
//                   break;
//               
//                 case idle1:
//                    leftmotor=0;
//                    rightmotor=0;
//                    state=backward;
//                    break;
//               
//               
//                case backward:
//                   PINRC13=0;// mapping the left backward
//                   PINRF5=11; 
//                   
//                   PINRD1=0;// mapping the right backward
//                   PINRB14=11; 
//                   state=wait1;
//                   break;
//                   
//                case idle2:
//                  leftmotor=0;
//                  rightmotor=0;
//                  state=forward;
//                  break;
//            case wait1:
//                if(val>511)
//                {
//                    leftmotor=1023;
//                    rightmotor=1023-2*val;
//                    
//                }
//                else if(val<511)
//                {
//                    leftmotor=2*val;
//                    rightmotor=1023;
//                   
//                }
//                else 
//                {
//                    leftmotor=1023;
//                    rightmotor=1023;
//                }
//                break;
//                
//                
//            
//            default:
//                state=forward;
//                    
//          }
//    
//       
//    }
//    
//    return 0;
//}
//
//
//void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void){
//    
//    IFS0bits.AD1IF = 0;
//    lcd=1;
//    val = ADC1BUF0;
//        
//}
//void __ISR(_CHANGE_NOTICE_VECTOR, IPL3SRS) _CNInterrupt(void){
//    IFS1bits.CNDIF = 0;         // Put Interrupt flag down
//    if(state==forward && SW1==0){
//      // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
//       state=idle1;
//    }
//    if(state==backward && SW1==0){
//     // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
//       state=idle2;
//    }
// 
//    
//}

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


    //TRISDbits.TRISD0 = 0;

    while (1) {


        switch (state) {
            case forward:
                PINRD1 = 11; //0b1011;
                PINRB14 = 0;
                PINRC4 = 11; // 0b1011
                PINRF5 = 0;
                state = wait1;
                break;
            case idle1:

                PINRD1 = 0; //0b1011;
                PINRB14 = 0;
                PINRC4 = 0; // 0b1011
                PINRF5 = 0;
                state = wait1;
                
                break;
            case backward:
                PINRD1 = 0; //0b1011;
                PINRB14 = 11;
                PINRC4 = 0; // 0b1011
                PINRF5 = 11;
                state = wait1;
                break;
            case idle2:

                PINRD1 = 0; //0b1011;
                PINRB14 = 0;
                PINRC4 = 0; // 0b1011
                PINRF5 = 0;
                state = wait1;
                
                break;
            
            case wait1:

                moveCursorLCD(0, 0);
                //printStringLCD("HELLO");
                //c = (char)val;
                //printCharLCD(c);
                v = val * 3.3 / 1023;
                sprintf(c, "%fV", v);
                printStringLCD(c);

                moveCursorLCD(1, 0);
                sprintf(adc, "%d", val);
                printStringLCD(adc);
                state = wait1;
                
                break;
        }

    }

    return 0;
}

void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void) {

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

void __ISR(_CHANGE_NOTICE_VECTOR, IPL4SRS) _CNInterrupt(void) {
    IFS1bits.CNDIF = 0; // Put Interrupt flag down
    if ((state == forward || state == wait1) && SW1 == 0) {
        // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
        state = idle1;
    }
    if ((state == backward || state == wait1) && SW1 == 0) {
        // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
        state = idle2;
    }

    if ((state == idle1 || state == wait1) && SW1 == 0) {
        // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
        state = backward;
    }
    
    if ((state == idle2 || state == wait1) && SW1 == 0) {
        // val is digital number from the equation" Vk=K(3.3-0)/1023+0  we could get voltage Vk=val*3.3/1023;
        state = forward;
    }
}