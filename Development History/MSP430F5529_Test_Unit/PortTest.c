#include <msp430.h>				

#define ENABLE_IO 0xFFFE
#define BUTTON11  BIT1
#define LED_ON  0x01
#define LED_OFF 0xFE
/**
 *
 */
void main(void) {
    /* Stops the watchdog timer */
	WDTCTL = WDTPW | WDTHOLD;

	PM5CTL0 = ENABLE_IO; // Required to use I/O


	P1DIR = LED_ON;   // P1.0 will be an output
	P1OUT = BUTTON11; // P1.1 will be an input
	P1REN = BUTTON11; // with a pull-up resistor

	while(1) {
	    if((BUTTON11 & P1IN) == 0) {
	        P1OUT = P1OUT | LED_ON;
	    }
	    else {
	        P1OUT = P1OUT & LED_OFF;
	    }
	}
}