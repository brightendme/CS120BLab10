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
unsigned char speakerSwitch = 0x00;
unsigned char speakerFrequency = 2;
enum ThreeLEDStates{TStart, TOne, TTwo, TThree }ThreeLEDStates;
void TickFct_ThreeLeds(){

//unsigned char A3 = PINA & 
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
	PORTB = blinkingLED | threeLEDs | speakerSwitch ;
}
enum switchPressState{SStart, SOn, SOff,SOnWait}switchPressState;
void switchPress(){
unsigned char A2 = ~PINA & 0x04;
	switch(switchPressState){
	
	case SStart:
		switchPressState = SOff;
		break;
	case SOff:
		if(A2 == 0x04){
		switchPressState = SOn;
		}
		else{
		switchPressState = SOff;
		}
		break;

	case SOn:
		switchPressState = SOnWait;
		break;
	case SOnWait:
		if(A2 == 0x04){
		switchPressState = SOn;
		}
		else{
			switchPressState = SOff;
		}

		break;
	default:
		break;

	}
	switch(switchPressState){
		case SStart:
			break;
		case SOn:
			speakerSwitch = 0x10;
			break;
		case SOnWait:
			speakerSwitch = 0x00;
			break;
		case SOff:
	//		speakerSwitch = 0x00;
			break;
		default:
			break;
	
	}
}
/*
enum buttonPressState{Bstart, Bup, Bdown, Bwait,BupWait,BdownWait}buttonPressState;
void buttonPress (){
unsigned char A0 =~PINA & 0x01;
unsigned char A1 =~PINA & 0x02;
	switch(buttonPressState){
		case Bstart:
			buttonPressState = Bwait;
			break;
		case Bwait:
			if((~PINA & 0x01) == 0x01){
				buttonPressState = Bup;
			}
			else if((~PINA & 0x02) == 0x02){
			//	buttonPressState = Bdown;
			//}
			//else{
			//	buttonPressState = Bwait;
			//}
			break;
		case Bup:
			buttonPressState = BupWait;
			break;
		case BupWait:
			if(A0 == 0x01){
				buttonPressState = BupWait;
			}
			else{
				buttonPressState = Bwait;
			}
			break;
		case Bdown:
			buttonPressState = BdownWait;
			break;
		case BdownWait:
			if(A1 == 0x02){
				buttonPressState = BdownWait;
			}
			else{
				buttonPressState = Bwait;
			}
			break;

		default:
			break;
		}
	
	switch(buttonPressState){
		case Bstart:
			break;
		case Bup:
			if(speakerFrequency < 8){
			speakerFrequency = speakerFrequency + 1;
			}
			break;
		case Bdown:
			if(speakerFrequency >= 2){
				speakerFrequency  = speakerFrequency- 1;
			}
			break;
		case Bwait:
			break;
		default:
			break;
	}
}
*/
			
int main(void) {
    /* Insert DDR and PORT initializations */ 
    DDRB = 0xFF; PORTB = 0x00;  
    DDRA = 0x00; PORTA = 0xFF;
    /* Insert your solution below */
    unsigned long BL_elapsedTime = 1000;
    unsigned long TL_elapsedTime = 300;
    unsigned long switchPressTime = 2;
    const unsigned long timerPeriod = 1;
       

    TimerSet(timerPeriod);
    TimerOn();
    ThreeLEDStates = TStart;
    BlinkLED_States = BLStart;
    switchPressState = SStart;
    //buttonPressState = Bstart;
    while (1) {
	    if(BL_elapsedTime >= 1000){
		    TickFct_BlinkLed();
		    BL_elapsedTime = 0;
	    }
	    if(TL_elapsedTime >= 300){
		    TickFct_ThreeLeds();
		    TL_elapsedTime = 0;
	    }
	    if(switchPressTime >= speakerFrequency){
		    switchPress();
		    switchPressTime = 0;
	    }
	    CombineLEDsSM();
	   // buttonPress();
	    while(!TimerFlag){}
	    TimerFlag = 0;
	    BL_elapsedTime += timerPeriod;
	    TL_elapsedTime += timerPeriod;
	    switchPressTime += timerPeriod;
    }
    return 1;
}
