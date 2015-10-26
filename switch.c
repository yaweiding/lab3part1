/*
 * File:   switch.c
 * Author: 
 *
 * 
 */
#include <xc.h>
#include "switch.h"

void initSW1(){
   
    TRISDbits.TRISD0=1;// Enable input for switch
    CNCONDbits.ON=1;       // Turn on CN device
    
    CNENDbits.CNIED0 = 1;       // Enable CN interrupt for pin
    IEC1bits.CNDIE = 1;         // Enable overall CN Interrupt
    IFS1bits.CNDIF = 0;         // Put Interrupt flag down
    
    CNPUDbits.CNPUD0=1;// Enable Interal Pull-up resistor
    
}
