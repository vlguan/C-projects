// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
//CSE13E Support Library
#include "BOARD.h"
#include "BOARD.h"
#include "Oled.h"
#include "Adc.h"
#include "Ascii.h"
#include "Buttons.h"
#include "Leds.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>





// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)

#define MAX_T_SIZE 1023
#define LONG_PRESS 5
#define TICK_COUNTER 5
#define LEDS_COUNT 8
#define WIN 3
#define PREV (-WIN - 1)


// **** Set any local typedefs here ****

typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING
} OvenState;

typedef enum {
    BAKE = 1, TOAST, BROIL
} OvenMode;

enum {
    TIME, TEMP
};

enum {
    TOGGLE1 = 0x01, TOGGLE2 = 0x02, TOGGLE3 = 0x04, TOGGLE4 = 0x08,
    TOGGLE5 = 0x10, TOGGLE6 = 0x20, TOGGLE7 = 0x40, TOGGLE8 = 0x80,
    ALL_LEDS = 0xFF
};

typedef struct {
    OvenState state;
    OvenMode mode; // Mode of oven
    unsigned int cookStartTime; // Time of cooking start
    unsigned int TiTmpselect : 1;
    unsigned int temp; // Temperature of oven
    unsigned int buttonPressTime; // Time of button press
    //add more members to this struct
} OvenData;


// **** Declare any datatypes here ****

struct AdcResult {
    uint8_t event;
    int16_t voltage;
};
// **** Define any module-level, global, or external variables here ****
static OvenData Oven;
static unsigned long int timer1;
static uint8_t TickEvent;
static struct AdcResult Adc;
static uint8_t ButtonStatus;
// **** Put any helper functions here ****

void resetOven(void)
{
    Oven.state = SETUP;
    Oven.mode = BAKE;
    Oven.temp = 350;
    Oven.cookStartTime = 1;
    Oven.TiTmpselect = TIME;
    Oven.buttonPressTime = 0;

}

void updateLED(int orig)
{
    int eight;
    int i = 1;
    eight = (Oven.cookStartTime) * LEDS_COUNT;
    LEDS_SET(ALL_LEDS);
    while (i <= LEDS_COUNT) {
        if (eight < orig * i) {
            LEDS_SET(LEDS_GET() ^ (TOGGLE8 >> (i)));
        }
        ++i;
    }
}

void OvenArt(int minute, int second, char strBuff[], OvenData ovenData)
{
    sprintf(strBuff, "|");
    if (ovenData.state >= COOKING && ovenData.mode != TOAST) {
        sprintf(strBuff + strlen(strBuff), "\x01\x01\x01\x01\x01|");
    } else {
        sprintf(strBuff + strlen(strBuff), "\x02\x02\x02\x02\x02|");
    }

    sprintf(strBuff + strlen(strBuff), "  Mode: ");
    if (ovenData.mode == BAKE) {
        sprintf(strBuff + strlen(strBuff), "Bake \n");
    } else if (ovenData.mode == TOAST) {
        sprintf(strBuff + strlen(strBuff), "Toast \n");
    } else if (ovenData.mode == BROIL) {
        sprintf(strBuff + strlen(strBuff), "Broil \n");
    }
    sprintf(strBuff + strlen(strBuff), "|     |");
    if (!ovenData.TiTmpselect && ovenData.mode == BAKE) {
        sprintf(strBuff + strlen(strBuff), " >");
    } else {
        sprintf(strBuff + strlen(strBuff), "  ");
    }
    sprintf(strBuff + strlen(strBuff), "Time: %2d:%02d\n", minute, second);
    sprintf(strBuff + strlen(strBuff), "|-----|");

    if (ovenData.TiTmpselect && ovenData.mode == BAKE) {
        sprintf(strBuff + strlen(strBuff), " >");
    } else {
        sprintf(strBuff + strlen(strBuff), "  ");
    }
    if (ovenData.mode != TOAST) {
        sprintf(strBuff + strlen(strBuff), "Temp: %-d\n", ovenData.temp);
    } else {
        sprintf(strBuff + strlen(strBuff), "            \n");
    }
    if (ovenData.state >= COOKING && ovenData.mode != BROIL) {
        sprintf(strBuff + strlen(strBuff), "|\x03\x03\x03\x03\x03|");
    } else {
        sprintf(strBuff + strlen(strBuff), "|\x04\x04\x04\x04\x04|");
    }
}

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData)
{
    int minute, second;
    char strBuff[80];

    minute = ovenData.cookStartTime / 60;
    second = ovenData.cookStartTime % 60;
    OvenArt(minute, second, strBuff, ovenData);

    OledDrawString(strBuff);
    OledUpdate();

    //update OLED here

}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void)
{
    //write your SM logic here.
    static unsigned int origCookTime;
    static uint8_t startTimer;

    switch (Oven.state) {
    case SETUP:
        LEDS_SET(~ALL_LEDS);
        // ADC Event
        if (Adc.event) {
            Adc.event = FALSE;
            if (Oven.TiTmpselect == TIME) {
                Oven.cookStartTime = (Adc.voltage >> 2) + 1;
            } else {
                Oven.temp = (Adc.voltage >> 2) + 300;
            }
            updateOvenOLED(Oven);
        }
        // Button Event
        if (ButtonStatus & BUTTON_EVENT_3DOWN) {
            Oven.buttonPressTime = timer1;
            Oven.state = SELECTOR_CHANGE_PENDING;
        }
        if (ButtonStatus & BUTTON_EVENT_4DOWN) {
            Oven.buttonPressTime = timer1;
            Oven.state = COOKING;
            origCookTime = Oven.cookStartTime;
            startTimer = TRUE;
            updateOvenOLED(Oven);
        }
        break;
    case SELECTOR_CHANGE_PENDING:
        if (ButtonStatus & BUTTON_EVENT_3UP) {
            if (timer1 - Oven.buttonPressTime < LONG_PRESS) {
                // Switch mode
                if (Oven.mode == BROIL) {
                    Oven.mode = BAKE;
                    Oven.temp = 350;
                } else {
                    Oven.mode++;
                }
                if (Oven.mode != BAKE) {
                    Oven.TiTmpselect = TIME;
                }
                if (Oven.mode == BROIL) {
                    Oven.temp = 500;
                }
            } else {
                // Switch settings selector for pot
                if (Oven.mode == BAKE) {
                    Oven.TiTmpselect = ~Oven.TiTmpselect;
                }
            }
            ButtonStatus = 0;
            updateOvenOLED(Oven);
            Oven.state = SETUP;
        }
        break;
    case COOKING:
        if (TickEvent) {
            TickEvent = FALSE;
            if (startTimer) {
                LEDS_SET(ALL_LEDS);
                startTimer = FALSE;
                break;
            } else {
                Oven.cookStartTime -= 1;
            }
            if (Oven.cookStartTime <= 0) {
                updateOvenOLED(Oven);
                Oven.state = SETUP;
                Oven.cookStartTime = origCookTime;
                LEDS_SET(~ALL_LEDS);
            } else {
                updateLED(origCookTime);
            }
            updateOvenOLED(Oven);
        }
        if (ButtonStatus & BUTTON_EVENT_4DOWN) {
            Oven.buttonPressTime = timer1;
            Oven.state = RESET_PENDING;
            ButtonStatus = 0;
        }
        break;
    case RESET_PENDING:
        if (TickEvent) {
            TickEvent = FALSE;
            Oven.cookStartTime -= 1;
            if (Oven.cookStartTime <= 0) {
                updateOvenOLED(Oven);
                Oven.state = SETUP;
                Oven.cookStartTime = origCookTime;
                LEDS_SET(~ALL_LEDS);
            } else {
                // Update LED
                updateLED(origCookTime);
            }
            updateOvenOLED(Oven);
        }

        if ((ButtonStatus & BUTTON_EVENT_4UP)) {
            if (timer1 - Oven.buttonPressTime < LONG_PRESS) {
                Oven.state = COOKING;
            } else {
                // End cooking, reset, update OLED
                updateOvenOLED(Oven);
                Oven.state = SETUP;
                Oven.cookStartTime = origCookTime;
            }
        }
        ButtonStatus = 0;

        break;
    }
    printf("state: %d mode: %d cookTime: %d\n", Oven.state, Oven.mode, Oven.cookStartTime);
}

int main()
{
    BOARD_Init();

    //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>

    printf("Welcome to vlguan's Lab07 (Toaster Oven).  Compiled on %s %s.\n", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here

    OledInit();
    AdcInit();
    ButtonsInit();
    LEDS_INIT();

    timer1 = 0;
    TickEvent = 0;
    resetOven();

    updateOvenOLED(Oven);
    while (1) {
        // Add main loop code here:
        // check for events
        if (Adc.event || ButtonStatus || TickEvent) {
            runOvenSM();
        }
        // on event, run runOvenSM()
    };
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;

    //add event-checking code here
    static int TickCount = 0;

    timer1++;
    if (TickCount++ == TICK_COUNTER) {
        TickEvent = TRUE;
        TickCount = 0;
    }
}

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //add event-checking code here
    ButtonStatus = ButtonsCheckEvents();

    if (AdcChanged()) {
        static int prevVal = PREV;
        Adc.event = TRUE;
        int buffer = AdcRead();

        if (abs(buffer - prevVal) > WIN) {
            // Corner case 1
            if (buffer < WIN) {
                buffer = 0;
            }
            // Corner case 2
            if (buffer > MAX_T_SIZE - WIN) {
                buffer = MAX_T_SIZE;
            }
            prevVal = buffer;
            Adc.voltage = buffer;
        }
    }
}