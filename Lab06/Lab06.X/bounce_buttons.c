// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CMPE13 Support Library
#include "BOARD.h"
#include "Buttons.h"
#include "Leds_Lab06.h"
// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>

// User libraries

// **** Set macros and preprocessor directives ****

struct button {
    uint8_t event;

};
uint8_t Button_Status;
uint8_t Switch_State;
char lights = 0x0;
uint8_t lights1 = 0x3;
uint8_t lights2 = 0xC;
uint8_t lights3 = 0x30;
uint8_t lights4 = 0xC0;
//char LED;
int x = 0, y = 0, z = 0, c = 0;
int one = 1;
struct button buttons1 = {FALSE};
// **** Declare any datatypes here ****

// **** Define global, module-level, or external variables here ****


uint8_t final;
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
    ButtonsInit();
    LEDS_INIT();
    printf("Welcome to CRUZID's lab6 part5 (bounce_buttons).  Compiled on %s %s.\n", __TIME__, __DATE__);
    while (1) {

        if (buttons1.event) {

            final = SWITCH_STATE_SW1 & Switch_State;
            if ((final == SWITCH_STATE_SW1) && (Button_Status == BUTTON_EVENT_1DOWN)) {
                x = one;
                if (x == one) {
                    lights = lights ^ lights1;
                    LEDS_SET(lights);
                    x = 0;
                }


            } else if ((final == 0) && (Button_Status == BUTTON_EVENT_1UP)) {
                x++;
                if (x > one) {
                    lights = lights ^ lights1;
                    LEDS_SET(lights);
                    x = 0;
                }
            }
            //                state2
            final = SWITCH_STATE_SW2 & Switch_State;
            if ((final == SWITCH_STATE_SW2) && (Button_Status == BUTTON_EVENT_2DOWN)) {
                x = one;
                if (x == one) {
                    lights = lights ^ lights2;
                    LEDS_SET(lights);
                    x = 0;
                }
            } else if ((final == 0) && (Button_Status == BUTTON_EVENT_2UP)) {
                x++;
                if (x > one) {
                    lights = lights ^ lights2;
                    LEDS_SET(lights);
                    x = 0;
                }
            }
            //state3
            final = SWITCH_STATE_SW3 & Switch_State;
            if ((final == SWITCH_STATE_SW3) && (Button_Status == BUTTON_EVENT_3DOWN)) {
                x = one;
                if (x == one) {
                    lights = lights ^ lights3;
                    LEDS_SET(lights);
                    x = 0;
                }
            } else if ((final == 0) && (Button_Status == BUTTON_EVENT_3UP)) {
                x++;
                if (x > one) {
                    lights = lights ^ lights3;
                    LEDS_SET(lights);
                    x = 0;
                }
            }
            //state4
            final = SWITCH_STATE_SW4 & Switch_State;
            if ((final == SWITCH_STATE_SW4) && (Button_Status == BUTTON_EVENT_4DOWN)) {
                x = one;
                if (x == one) {
                    lights = lights ^ lights4;
                    LEDS_SET(lights);
                    x = 0;
                }
            } else if ((final == 0) && (Button_Status == BUTTON_EVENT_4UP)) {
                x++;
                if (x > one) {
                    lights = lights ^ lights4;
                    LEDS_SET(lights);
                    x = 0;
                }
            }
        }
    }
    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     ***************************************************************************************************/
}

/**
 * This is the interrupt for the Timer1 peripheral. It should check for button events and stores them in a
 * module-level variable.
 * 
 * You should not modify this function for ButtonsTest.c or bounce_buttons.c!
 */
void __ISR(_TIMER_1_VECTOR, ipl4auto) Timer1Handler(void)
{
    // Clear the interrupt flag.
    IFS0bits.T1IF = 0;

    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    Button_Status = ButtonsCheckEvents();
    if (Button_Status) {
        buttons1.event = TRUE;
    }
    Switch_State = SWITCH_STATES();


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks
     **************************************************************************************************/

}