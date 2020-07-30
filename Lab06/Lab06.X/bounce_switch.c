
// **** Include libraries here ****
// old bounce
// Standard libraries
#include <stdio.h>
#include <GenericTypeDefs.h>


//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries
#include "Leds_Lab06.h"

// **** Declare any datatypes here ****

struct Timer {
    uint8_t event;
    int16_t timeRemaining;
};

// **** Define global, module-level, or external variables here ****
static struct Timer Timer1 = {FALSE, 8};
int LEFT = 1;

// **** Declare function prototypes ****

int main(void)
{
    BOARD_Init();

    // Configure Timer 1 using PBCLK as input. This default period will make the LEDs blink at a
    // pretty reasonable rate to start.
    T1CON = 0; // everything should be off
    T1CONbits.TCKPS = 1; // 1:8 prescaler
    PR1 = 0xFFFF; // interrupt at max interval
    T1CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T1IF = 0; //clear the interrupt flag before configuring
    IPC1bits.T1IP = 4; // priority of  4
    IPC1bits.T1IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T1IE = 1; // turn the interrupt on

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    printf("Welcome to vlguan's lab6 part2 (bounce_switch).  Compiled on %s %s.\n", __TIME__, __DATE__);
    //    int currentState;
    unsigned char x = 0x01;
    int currentState = 0;
    LEDS_INIT();
    while (1) {
        if (Timer1.event == TRUE) {
            Timer1.event = FALSE;
            if (currentState == LEFT) {
                if (x == 0x01) {
                    currentState = 0;

                    continue;
                }
                if (x != 0x01) {
                    x /= 0x02;
                }

                LEDS_SET(x);

            } else {
                if (x == (0x80)) {

                    currentState = 1;
                    LEDS_SET(x);
                    // x = 0x80;
                    continue;
                }

                LEDS_SET(x);

                x *= 0x02;

            }
        }
        //poll timer events and react if any occur
    }

}
/***************************************************************************************************
 * Your code goes in between this comment and the preceding one with asterisks
 **************************************************************************************************/

/**
 * This is the interrupt for the Timer1 peripheral. It will trigger at the frequency of the peripheral
 * clock, divided by the timer 1 prescaler and the interrupt interval.
 * 
 * It should not be called, and should communicate with main code only by using module-level variables.
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/

    Timer1.timeRemaining -= 7;
    if (SWITCH_STATES() == 0x8 || SWITCH_STATES() == 0x4 || SWITCH_STATES() == 0x2 || SWITCH_STATES() == 0x1) {
        Timer1.timeRemaining += 1;
    } else if (SWITCH_STATES() == 0x9 || SWITCH_STATES() == 0x5 || SWITCH_STATES() == 0x3 || SWITCH_STATES() == 0xA
            || SWITCH_STATES() == 0xC || SWITCH_STATES() == 0x6) {
        Timer1.timeRemaining += 2;
    } else if (SWITCH_STATES() == 0xB || SWITCH_STATES() == 0x7 || SWITCH_STATES() == 0xD || SWITCH_STATES() == 0xE) {
        Timer1.timeRemaining += 3;
    } else if (SWITCH_STATES() == 0xF) {
        Timer1.timeRemaining += 4;
    }
    if (Timer1.timeRemaining < 0) {
        Timer1.timeRemaining = 8;
        Timer1.event = TRUE;
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}
