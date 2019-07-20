#include <msp430.h>				

#define   RED_LED BIT0
#define GREEN_LED BIT7
/**
 * Simple blink program to test whether the setup proper by
 * testing whether the red and green LEDs are functional
 */
void main(void) {
    /* Stops the watchdog timer */
	WDTCTL = WDTPW | WDTHOLD;

	/* Configures red and green LEDs as output */
	P1DIR |= RED_LED;
	P4DIR |= GREEN_LED;

	/* Prevents optimization */
	volatile unsigned int i;

	while(1) {
	    /* Toggles the red and green LEDs */
		P1OUT ^= RED_LED;
		P4OUT ^= GREEN_LED;

		/* Manual delay */
		for(i = 0; i < 20000; i++);
	}
}