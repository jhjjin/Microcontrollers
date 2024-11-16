
#include <stdio.h>      // Standard input-output library
#include <stdlib.h>     // Standard library
#include "config.h"     // Configuration header file

#define POTENTIOMETER  PORTBbits.RB1    // Define Potentiometer as PORTB bit 1
#define LED PORTDbits.RD1               // Define LED as PORTD bit 1

int main()  
{
    ADCON1 = 0x0f;      // Set all pints as digital I/O
    TRISD = 0x00;       // Set PORTD as output
    TRISB = 0x0f;       // Set PORTB as input

    while (1)           // Infinite loop
    {
 
        LED = ~POTENTIOMETER;   // Set LED to the inverse of Potentiometer
    }

    return (EXIT_SUCCESS);      // Return success
}
