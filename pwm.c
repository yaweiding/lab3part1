/* 
 * File:   pwm.h
 * Author: gvanhoy
 *
 * Created on October 8, 2015, 9:14 AM
 */

#include <xc.h>
#include "pwm.h"

//
//#define  PINRD3  RPD3Rbits.RPD3R    // map for OCD4
//#define  PINRD11 RPD11Rbits.RPD11R // map for OCD4
//
//#define PINRD1   RPD1Rbits.RPD1R   // map for OCD2
//#define PINRD5   RPD5Rbits.RPD5R   // map for OCD2

void initPWM(){ // the problem is we need to figure out which ocd is for left or right 
   
    // we could change the mapping pins in software to change the direction  
    // RPD1Rbits.RPD1R=0b1011;// map for ocd2
     //RPD5Rbits.RPD5R=0;
     PINRD1=11;      //0b1011;
     PINRB14=0;
    OC2R = 0; // Initialize primary Compare Register
    OC2RS = 0; // Initialize secondary Compare Register
    OC2CONbits.OCM = 6; // Configure for PWM mode Fault pin disabled
    OC2CONbits.OCTSEL = 1; // Timer3
    OC2CONbits.ON = 1; // Enable OC1
    
    // we could change the mapping pins in software to change the direction 
   
    PINRC4=11; // 0b1011
    PINRF5=0;
    OC4R = 0; // Initialize primary Compare Register
    OC4RS = 0; // Initialize secondary Compare Register
    OC4CONbits.OCM = 6; // Configure for PWM mode
    OC4CONbits.OCTSEL = 1; // timer3
    OC4CONbits.ON = 1; // Turn off OC4 while doing setup.
    
    
    
    
    

   
           
}