/* Disables the watchdog timer */
WDTCTL = WDTPW | WDTHOLD;

/* Pets the watchdog timer */
WDTCTL = WDTPW | WDTCNTCL;