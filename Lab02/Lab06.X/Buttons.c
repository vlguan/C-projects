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


// look at button states and see if that change for four counts keep track of the last 4 states and see if curent state is equal to the previous state
    void ButtonsInit(void){
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
uint8_t ButtonsCheckEvents(void){
if (cooldown > 0){
    cooldown--;
    return BUTTON_EVENT_NONE;
}
uint8_t currentReading = BUTTON_STATES();
uint8_t returnEvent = BUTTON_EVENT_NONE;

if (currentReading == prevState) {
    return BUTTON_EVENT_NONE;
}else{
    prevSate = currentReading;
    cooldown = BUTTONS_DEBOUNCE_PERIOD;
    return returnEvent;
}
   
}
