/*
 * File:   Buttons.c
 * Author: vince
 *
 * Created on February 18, 2020, 5:19 PM
 */

#include "BOARD.h"
#include "Buttons.h"
#include "GenericTypeDefs.h"
#include <stdio.h>

static uint8_t prevState;
static uint8_t cooldown;
// look at button states and see if that change for four counts keep track of the last 4 states and see if curent state is equal to the previous state

void ButtonsInit(void)
{
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
    prevState = 0x0;
    cooldown = 0;
}

/**
 * ButtonsCheckEvents function checks the current button states and returns
 *  any events that have occured since its last
 * call.  This function should be called repeatedly in a Timer ISR, though it can
 * be called in main() during testing.
 *
 * In normal use, this function should only be used after ButtonsInit().
 * 
 * This function should assume that the buttons start in an off state with 
 * value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, 
 * BUTTONS_EVENT_NONE should be returned. 
 * 
 * @return  Each bit of the return value corresponds to one ButtonEvent flag,
 *          as described in Buttons.h.  If no events are detected, BUTTONS_EVENT_NONE is returned.
 * 
 * Note that more than one event can occur simultaneously, though this situation is rare.
 * To handle this, the output should be a bitwise OR of all applicable event flags. 
 * For example, if button 1 was released at the same time that button 2 was pressed,
 * this function should return (BUTTON_EVENT_1UP | BUTTON_EVENT_2DOWN) 
 * 
 */
uint8_t ButtonsCheckEvents(void)
{
    if (cooldown > 0) {
        cooldown--;
        return BUTTON_EVENT_NONE;
    }

    uint8_t currentReading = BUTTON_STATES();
    uint8_t returnEvent = BUTTON_EVENT_NONE;

    if (currentReading == prevState) {
        return BUTTON_EVENT_NONE;
    } else {
        if (((currentReading & BUTTON_STATE_1) == BUTTON_STATE_1)&&((prevState & BUTTON_STATE_1) == 0)) {
            returnEvent = returnEvent | BUTTON_EVENT_1DOWN;
        } else if (((currentReading & BUTTON_STATE_1) == 0x0)&& ((prevState & BUTTON_STATE_1) == BUTTON_STATE_1)) {
            returnEvent = returnEvent | BUTTON_EVENT_1UP;
        }
        if (((currentReading & BUTTON_STATE_2) == BUTTON_STATE_2)&&((prevState & BUTTON_STATE_2) == 0)) {
            returnEvent = returnEvent | BUTTON_EVENT_2DOWN;
        } else if (((currentReading & BUTTON_STATE_2) == 0x0)&& ((prevState & BUTTON_STATE_2) == BUTTON_STATE_2)) {
            returnEvent = returnEvent | BUTTON_EVENT_2UP;
        }
        if (((currentReading & BUTTON_STATE_3) == BUTTON_STATE_3)&&((prevState & BUTTON_STATE_3) == 0)) {
            returnEvent = returnEvent | BUTTON_EVENT_3DOWN;
        } else if (((currentReading & BUTTON_STATE_3) == 0x0)&& ((prevState & BUTTON_STATE_3) == BUTTON_STATE_3)) {
            returnEvent = returnEvent | BUTTON_EVENT_3UP;
        }
        if (((currentReading & BUTTON_STATE_4) == BUTTON_STATE_4)&&((prevState & BUTTON_STATE_4) == 0)) {
            returnEvent = returnEvent | BUTTON_EVENT_4DOWN;
        } else if (((currentReading & BUTTON_STATE_4) == 0x0)&& ((prevState & BUTTON_STATE_4) == BUTTON_STATE_4)) {
            returnEvent = returnEvent | BUTTON_EVENT_4UP;
        }

        prevState = currentReading;
        cooldown = BUTTONS_DEBOUNCE_PERIOD;
        return returnEvent;
    }

}
