/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <timer.h>
/*
volatile unsigned char TimerFlag = 0;
void TimerISR(){
	TimerFlag = 1;
}
*/
unsigned char blinkingLED = 0x00;
unsigned char threeLEDs = 0x00; 
enum ThreeLEDStates{TStart, TOne, TTwo, TThree }ThreeLEDStates;
void TickFct_ThreeLeds(){
//state transitions
   switch(ThreeLEDStates){
   
	case TStart:
		ThreeLEDStates = TOne;
		break;

	case TOne:
		ThreeLEDStates = TTwo;
	     	break;
	
	case TTwo:
		ThreeLEDStates = TThree;
		break;

	case TThree:
		ThreeLEDStates = TOne;
		break;

	default:
		break;
   }


   switch(ThreeLEDStates){
	   //state actions
	case TStart:
		break;
	
	case TOne:
		threeLEDs = 0x01;
		break;

	case TTwo:
		threeLEDs = 0x02;
		break;
	
	case TThree:
		threeLEDs = 0x04;
		break;

	default:
		break;
   }

}

enum BlinkLED_States{BLStart, BLOn, BLOff }BlinkLED_States;
void TickFct_BlinkLed(){
   switch(BlinkLED_States){
	case BLStart:
		BlinkLED_States = BLOn;
		break;

	case BLOn:
		BlinkLED_States = BLOff;
		break;

	case BLOff:
		BlinkLED_States = BLOn;
		break;
	
	default:
		break;
   }

   switch(BlinkLED_States){
	case BLStart:
		break;
     	case BLOn:
		blinkingLED = 0x08;
		break;
	case BLOff:
		blinkingLED = 0x00;
		break;
	default:
		break;

   }
}

void CombineLEDsSM(){
	PORTB = blinkingLED | threeLEDs ;
}	
int main(void) {
    /* Insert DDR and PORT initializations */ 
    DDRB = 0xFF; PORTB = 0x00;  
    /* Insert your solution below */
    unsigned long BL_elapsedTime = 1000;
    unsigned long TL_elapsedTime = 1000;
    const unsigned long timerPeriod = 1;

    TimerSet(timerPeriod);
    TimerOn();
    ThreeLEDStates = TStart;
    BlinkLED_States = BLStart;
    while (1) {
	    if(BL_elapsedTime >= 1000){
		    TickFct_BlinkLed();
		    BL_elapsedTime = 0;
	    }
	    if(TL_elapsedTime >= 1000){
		    TickFct_ThreeLeds();
		    TL_elapsedTime = 0;
	    }
	    CombineLEDsSM();
	    while(!TimerFlag){}
	    TimerFlag = 0;
	    BL_elapsedTime += timerPeriod;
	    TL_elapsedTime += timerPeriod;
    }
    return 1;
}
