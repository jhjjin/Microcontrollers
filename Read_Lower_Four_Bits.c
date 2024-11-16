
#include <stdio.h>   // Standard input - output library
#include <stdlib.h>  // Standard library
#include "config.h"  // Configuration header file

//RB0~RB3 : Switch (input)
// RA0~6 : common Anode SSEG (Output)

/*
 * 
 */

int main() 
{
    char sseg[16] = {0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0,0x8F, 0x80, 0x84, 0x88, 0x80, 0xB1, 0xC2, 0xB0, 0xB8}; // 7 Segment pattern for 0~9, A~F

    
    ADCON1 = 0x0f; // Configure all port pins as digital I/0
    TRISA = 0x00;  // Set PORTA as output to drive the 7 segment display
    TRISB  = 0x0f; // Set PORTB as input to read switches
   
    while(1) // Infinite loop
    {
       
        PORTA = sseg[PORTB & 0x0f]; // Read lower four bits from PORTB and use them to index into the sseg array. Write the pattern to PORTA
    }
 
    return (EXIT_SUCCESS); // Return success. 
}

