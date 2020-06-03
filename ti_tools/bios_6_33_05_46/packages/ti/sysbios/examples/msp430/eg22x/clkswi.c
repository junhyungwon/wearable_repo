/*
 *  ======== clkswi.c ========
 *  This example increments some counters in some Swis and Clock functions and
 *  dumps their values every 100 Clock ticks.
 * 
 *  SysMin's output is routed through the UART and is visible in a 
 *  a corresponding CCS Terminal window. 
 */
 
/*
 * This header file is included for referencing P1DIR and P1OUT to control an 
 * LED connected to P1.0.  For this purpose, this same header file can be used 
 * with other msp430x22x devices and development boards that follow this
 * common convention.
 */
#include <msp430f2274.h>

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Diags.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Swi.h>

extern const Swi_Handle swiA;
extern const Swi_Handle swiB;

Uns swi0Count = 0;
Uns swi1Count = 0;
Uns clk0Count = 0;
Uns clk1Count = 0;

UInt8 cmd = 0;
Bool statusEnabled = TRUE;

/*
 *  ======== swiFxnA ========
 */
Void swiFxnA(UArg arg1, UArg arg2)
{
    swi0Count++;

    if ((swi0Count & 0xf) == 0) {
        P1OUT ^= 0x2;  /* toggle LED 2 */
    }
}

/*
 *  ======== swiFxnB ========
 */
Void swiFxnB(UArg arg1, UArg arg2)
{
    swi1Count++;
    
    if ((swi1Count & 0xf) == 0) {
        P1OUT ^= 0x1;  /* toggle LED 1 */
    }
}

/*
 *  ======== clk0Fxn ========
 */
Void clk0Fxn(UArg arg) 
{
    clk0Count++;
    Swi_post(swiA);
}

/*
 *  ======== clk1Fxn ========
 */
Void clk1Fxn(UArg arg) 
{
    clk1Count++;
    Swi_post(swiB);
}

/*
 *  ======== main ========
 */
Int main()
{
    System_printf("Starting SYS/BIOS ...\n");

    BIOS_start();

    return 0;
}

/*
 *  ======== idl0Fxn ========
 */
Void idl0Fxn()
{
    /* check for command input from serial port */
    if (cmd != 0) {
        switch (cmd) {
            /* toggle automatic status dumping */
            case 's':
               if (statusEnabled == TRUE) { 
                    statusEnabled = FALSE;
                    System_printf("Status reporting disabled.\n");
                }
                else {
                    statusEnabled = TRUE;
                    System_printf("Status reporting enabled.\n");
                }
                break;
            default:
                break;
        }
        cmd = 0;
    }

    /* dump counts every 100 Clock ticks */
    if ((statusEnabled == TRUE)
        && Clock_getTicks() && (Clock_getTicks() % 100 == 0)) {     
        System_printf("status:\n\tswi0Count: %u, swi1Count: %u\n",
                    swi0Count, swi1Count);
        System_printf("\tclk0Count: %u, clk1Count: %u\n",
                    clk0Count, clk1Count);
    }
    
    /* 
     * Flush characters in SysMin buffer 
     */
    System_flush();
}

/*
 *  ======== myPutCharFxn ========
 *  Output a character
 *
 *  This function is called by System_flush() when the SysMin buffer 
 *  has characters to be sent.
 *  
 *      SysMin.outputFxn = "&myPutCharFxn";
 * 
 *  This function can take a while to complete since
 *  it spin-waits between characters while waiting for 
 *  the transmitter to become available.
 * 
 */
Void myPutCharFxn(char *ch, UInt num)
{
    while(num--) {
        while (!(IFG2 & UCA0TXIFG)) {   /* USCI_A0 TX buffer ready? */
            ;
        }
        UCA0TXBUF = *ch;                 /* TX -> ch */

        /* add CR to LFs */
        if (*ch++ == '\n') {
            while (!(IFG2 & UCA0TXIFG)) {
                ;
            }
            UCA0TXBUF = '\r';
        }
    }
}

/*
 *  ======== myInit ========
 *  Initialize peripherals
 *
 *  This function is called prior to main because it is configured
 *  in clkswi.cfg as a 'firstFxn' in the Startup module:
 * 
 *      Startup.firstFxn[len] = "&myInit";
 */
Void myInit(void)
{
    WDTCTL = WDTPW + WDTHOLD;       /* Stop watchdog timer */

    P1DIR |= 0x03;                  /* Set P1.0,1 to output direction */
    P1OUT = 0;                      /* turn off LEDs */

    /* 
     * delay loop to ensure proper startup before increasing the DCO from the
     * reset startup value of 1MHz.
     * This is typically tailored to the power supply used, and in this case
     * is overkill for safety due to wide distribution.
     */
    {volatile int i; for (i = 0; i < 0xFFFF; i++){}}
    
    /* Configure DCO for 8MHz operation */

    BCSCTL1 = CALBC1_8MHZ;          /* Set DCO to the default of Simpliciti */
    DCOCTL = CALDCO_8MHZ;
    BCSCTL3 |= LFXT1S_2;            /* LFXT1 = VLO */

    /* init the UART (see Table 15-4, Section 15.3.13 MSP430 User's guide) */
    P3SEL = 0x30;           /* P3.4,5 = USCI_A0 TXD/RXD */
    UCA0CTL1 = UCSSEL_2;    /* SMCLK */

    UCA0BR0 = 0x41;         /* 8MHz DCO 9600 (UCBRx = 833) */
    UCA0BR1 = 0x03;         /* 8MHz DC0 9600 */
    UCA0MCTL = UCBRS_2;     /* 8MHz DC0 Modulation (UCBRSx = 2: UCOS16 = 0, UCBRFx = 0) */

    UCA0CTL1 &= ~UCSWRST;   /* Initialize USCI state machine */
    IE2 |= UCA0RXIE;        /* Enable USCI_A0 RX interrupt */
}

/*
 *  ======== USCI0RX_ISR ========
 *  The USCI recieve interrupt function
 *
 *  This function is called when a character is available in the USCI
 *  RX buffer.  Since this interrupt is directly plugged we must avoid
 *  BIOS calls that interact with the scheduler.  So, we simply copy
 *  the character into a "command buffer", which is read, processed, 
 *  and cleared by the idle function.
 */
 #pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    cmd = UCA0RXBUF;    /* read the character and copy to cmd */
}

