
#include <stdio.h>      // Standard input -output library
#include <stdlib.h>     // Standard library 
#include "config.h"     // Configuration header file 

#define LEDC  PORTCbits.RC0     // Define LEDC as RC0 bit of PORTC
#define LEDD  PORTDbits.RD0     // Define LEDD as RD0 bit of PORTD
#define SWITCHC PORTCbits.RC1   // Define SWITCHC as RC1 bit of PORTC
#define SWITCHD PORTDbits.RD1   // Define SWITCHD as RD1 bit of PORTD

/*
 * 
 */
int main() 
{
    ADCON1  = 0x0f;         // Configure pins as digital I/O
    TRISCbits.TRISC0 = 0;   // Set RC0 as output
    TRISDbits.TRISD0 = 0;   // Set RD0 as output
    TRISCbits.TRISC1 = 1;   // Set RC1 as input
    TRISDbits.TRISD1 = 1;   // Set RD1 as input
    
    while(1)                // Infinite loop
    {
        if(SWITCHC == 1)    // Check the SWITCHC is high
        {
             LEDC = 0;      // Turn OFF LEDC
        }
        else 
        {
            LEDC = 1;       // Turn ON LEDC
        }
        
        if(SWITCHD == 1)    // Check the SWITCHD is high
        {
            LEDD = 0;       // Turn OFF LEDD
        }
        else 
        {
            LEDD = 1;       // Turn ON LEDD
        }
            
    }
    return (EXIT_SUCCESS);  // Return Success
}

