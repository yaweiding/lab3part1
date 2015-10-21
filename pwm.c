/* 
 * File:   pwm.h
 * Author: gvanhoy
 *
 * Created on October 8, 2015, 9:14 AM
 */

#include <xc.h>


void initPWM(){
    //RPD1Rbits.RPD1R = 0b1011; // map OC2 to RD1 how to map
   
    OC1R = 0; // Initialize primary Compare Register
    OC1RS = 0; // Initialize secondary Compare Register
    OC1CONbits.OCM = 6; // Configure for PWM mode
    OC1CONbits.OCTSEL = 1; // 
    OC1CONbits.ON = 1; // Turn off OC4 while doing setup.
    
    
    OC2R = 0; // Initialize primary Compare Register
    OC2RS = 0; // Initialize secondary Compare Register
    OC2CONbits.OCM = 6; // Configure for PWM mode Fault pin disabled
    OC2CONbits.OCTSEL = 1; // Timer3
    OC2CONbits.ON = 1; // Enable OC1
  
    
    

   
           
}