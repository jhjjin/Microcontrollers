#include <stdio.h>  // Standard input-output library
#include <stdlib.h> // Standard library
#include "config.h" // Configuration header file

#define _XTAL_FREQ 1000000      // Define the oscillator frequency
#define SEGA    PORTAbits.RA0   // Define segment A as RA0
#define SEGB    PORTAbits.RA1   // Define segment B as RA1
#define SEGC    PORTAbits.RA2   // Define segment C as RA2
#define SEGD    PORTAbits.RA3   // Define segment D as RA3
#define SEGE    PORTAbits.RA4   // Define segment E as RA4
#define SEGF    PORTAbits.RA5   // Define segment F as RA5
#define SEGG    PORTAbits.RA6   // Define segment G as RA6

// RA0 - A, RA1 - B, RA2 - C, RA3- D, RA4 - E, RA5 - F, RA6 -G (output SSEG) / RD0 - #4 switch , RD1 - #3 switch (input Switchs)
// Using Common Anode for 7 Segment
/*
 * 
 */
int main() 
{
    ADCON1 = 0x0f;  // Configure pins as digital I/O
    TRISA  = 0x00;  // Set PORTA as output
    TRISD  = 0xff;  // Set PORTD as input

    while(1)        // Infinite loop
    {
        switch(PORTD & 3)  // Mask for only RD0 and RD1. Using switch select the case.
        {
            case 0:             // Case 0 : Display Off
                PORTA = 0xff;   // Turn off all segments 
                __delay_ms(500);// Delay for 500 milliseconds
                break;          //  Break to exit switch statement
                
            case 1:              // Case 1: Toggle G
                PORTA = 0xff;    // Turn off all segments 
                SEGG = 0;        // Turn on segment G 
                __delay_ms(500); // Delay for 500 milliseconds
                SEGG = 1;        // Turn off segment G (set high for common anode)
                __delay_ms(500); // Delay for 500milliseconds
                break;           // Break to exit switch statement
                
            case 2:          // Case 2: Rotate clockwise
                SEGG = 1;    // Turn off the segment G
                
                SEGA = 0; __delay_ms(500); SEGA = 1;    // Turn on segment A -> Delay -> Turn off segment A
                SEGB = 0; __delay_ms(500); SEGB = 1;    // Turn on segment B -> Delay -> Turn off segment B
                SEGC = 0; __delay_ms(500); SEGC = 1;    // Turn on segment C -> Delay -> Turn off segment C
                SEGD = 0; __delay_ms(500); SEGD = 1;    // Turn on segment D -> Delay -> Turn off segment D
                SEGE = 0; __delay_ms(500); SEGE = 1;    // Turn on segment E -> Delay -> Turn off segment E
                SEGF = 0; __delay_ms(500); SEGF = 1;    // Turn on segment F -> Delay -> Turn off segment F
                break;                                  // Break to exit switch statement
                
            case 3:           // Case 3: Rotate Counter-Clockwise
                SEGG = 1;     // Turn off segment G
                
                SEGF = 0; __delay_ms(500); SEGF = 1;    // Turn on segment F -> Delay -> Turn off segment F
                SEGE = 0; __delay_ms(500); SEGE = 1;    // Turn on segment E -> Delay -> Turn off segment E
                SEGD = 0; __delay_ms(500); SEGD = 1;    // Turn on segment D -> Delay -> Turn off segment D
                SEGC = 0; __delay_ms(500); SEGC = 1;    // Turn on segment C -> Delay -> Turn off segment C
                SEGB = 0; __delay_ms(500); SEGB = 1;    // Turn on segment B -> Delay -> Turn off segment B
                SEGA = 0; __delay_ms(500); SEGA = 1;    // Turn on segment A -> Delay -> Turn off segment A
                break;                                  // Break to exit switch statement
                
            default:          // Default case 
                PORTA = 0xff; // Turn off all segment 
                break;        // Break to exit switch statement
        }
    }

    return (EXIT_SUCCESS);    
}
