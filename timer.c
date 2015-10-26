/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include "timer.h"
#define Fcy 8000000
/*-------------------------------------------------------------------------------------------------------*/
// use timer1 to count 10ms
/*------------------------------------------------------------------------------------------------------*/
void initTimer1(){
    TMR1=0;// clear timer1
    PR1=(Fcy*0.01)/256-1;
    T1CONbits.ON=1;// enable timer1
    T1CONbits.TCKPS=3;// pre-scalar is equal to 256
    T1CONbits.TCS=0;// internal oscillator 
    IFS0bits.T1IF=0;// Put down flag
    IPC1bits.T1IP=3;// configure the interrupt priority 
    IEC0bits.T1IE=1; // Enable the interrupt
}


/*-------------------------------------------------------------------------------------------------------*/
// use timer2 to delay 1us
/*------------------------------------------------------------------------------------------------------*/
void initTimer2(){
    TMR2=0;// clear timer2
    T2CONbits.ON=1;// enable timer2
    T2CONbits.TCKPS=0;// pre-scalar is equal to 1
    T2CONbits.TCS=0;// internal oscillator 
    IFS0bits.T2IF=0;// Put down flag
    //IPC2bits.T2IP=3;// configure the interrupt priority 
    IEC0bits.T2IE=1; // Enable the interrupt
}

//Uses timer 2 the oscillator is 8M (*20/2/12/1)
void delayUs(unsigned int delay){
 //TODO: Create a delay for "delay" micro seconds using timer 2

    TMR2=0;// clear timer1
    PR2= delay*8-1;// delay 1Us 
    IFS0bits.T2IF=0;// Put flag down
    T2CONbits.ON=1;// enable timer2
    while(IFS0bits.T2IF==0);
    T2CONbits.ON=0;//turn off timer2
   
}
void delayMs(unsigned int delay){
 //TODO: Create a delay for "delay" micro seconds using timer 2

    TMR2=0;// clear timer1
    PR2= delay*8000-1;// delay 1Ms 
    IFS0bits.T2IF=0;// Put flag down
    T2CONbits.ON=1;// enable timer2
    while(IFS0bits.T2IF==0);
    T2CONbits.ON=0;//turn off timer2
   
}

void getTimeString(int timer, char *string )
{   //char *string;
    int FF;
    int SS;
    int MM;
    FF=timer%100;// mod(100) is 0.01s
    SS=(timer/100)%60; // mod(60) is 1s
    MM=((timer/100) - SS) / 60; // 
    // sprintf write the formate data to string
    sprintf(string,"%02d:%02d:%02d",MM,SS,FF);
}
/*-------------------------------------------------------------------------------------------------------*/
// use timer3 to compare output
/*------------------------------------------------------------------------------------------------------*/
void initTimer3(){
    TMR3=0;// clear timer2
    PR3=1023;
    T3CONbits.ON=1;// enable timer2
    T3CONbits.TCKPS=0;// pre-scalar is equal to 1
    T2CONbits.TCS=0;// internal oscillator 
    IFS0bits.T2IF=0;// Put down flag
    //IPC2bits.T2IP=3;// configure the interrupt priority 
    IEC0bits.T2IE=1; // Enable the interrupt
}
