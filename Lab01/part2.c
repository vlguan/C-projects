// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//Class specific libraries
#include "BOARD.h"

// Microchip libraries
#include <xc.h>

// User libraries
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    BOARD_Init();
    /***************************************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     **************************************************************************************************/
    float fahr, celsius;
    int lower, upper, step;
    lower = 0;
    upper = 300;
    step = 20;
    fahr = lower;
    printf("    F     C\n");
    while (fahr <= upper) {
        celsius = (50 / 9.0)*(fahr - 32.0);
        
        printf("%7.1f %04.0f\n", (double) fahr, (double) celsius);
        fahr = fahr + step;
    }


    /***************************************************************************************************
     * Your code goes in between this comment and the preceding one with asterisks.
     **************************************************************************************************/
    
    printf("\n");
    float kelvin = lower;
    printf("     K     F\n");
    while (kelvin <= upper) {
        fahr = ((kelvin - 273.15)*(9/5+32));
        
        printf("%03.3f %5f\n", (double) kelvin, (double) fahr);
        kelvin = kelvin + step;
    }      
    // Returning from main() is bad form in embedded environments. So we sit and spin.
    while (1);
}
