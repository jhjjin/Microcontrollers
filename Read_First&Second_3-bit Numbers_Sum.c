#include <stdio.h>  // Standard input-output library
#include <stdlib.h> // Standard library
#include "config.h" // Configuration header file


// PORTD 0~6 is input for switches,  6 switches --> 3bits + 3bits = sum 
// RC0 - A, RC1 - B, RC2 - C , RC3 - D, RC4 - E, RC5 - F, RC6 - G
// Using Common Anode for 7 segment

#define SWITCH0 PORTDbits.RD0    // Define SWITCH0 as RD0 bit of PORTD
#define SWITCH1 PORTDbits.RD1    // Define SWITCH1 as RD1 bit of PORTD
#define SWITCH2 PORTDbits.RD2    // Define SWITCH2 as RD2 bit of PORTD
#define SWITCH3 PORTDbits.RD3    // Define SWITCH3 as RD3 bit of PORTD
#define SWITCH4 PORTDbits.RD4    // Define SWITCH4 as RD4 bit of PORTD
#define SWITCH5 PORTDbits.RD5    // Define SWITCH5 as RD5 bit of PORTD

int main() 
{
    
    unsigned char sseg[16] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88,  0x83, 0xC6,  0xA1, 0x86}; 
    // Seven-segment display encoding for numbers 0-9 and A-E (common anode)

    ADCON1 = 0xff; // Configure all port pins as digital I/O
    TRISC = 0x00;  // Set PORTC as output to drive the 7-segment display 
    TRISD = 0xff;  // Set PORTD as input to read switches
    PORTC = 0xff;  // Initialize PORTC to 0xff to turning of the each segment

    while (1)       // Infinite loop 
    {
        
        unsigned char firstNumber = ((SWITCH0 << 2) | (SWITCH1 << 1) | (SWITCH2)); // Read the first 3-bit number from switches 0, 1, and 2 using left shift operator
        unsigned char secondNumber = ((SWITCH3 << 2) | (SWITCH4 << 1) | (SWITCH5));// Read the second 3-bit number from switches 3, 4, and 5 using left shift operator
        unsigned char sum = firstNumber + secondNumber; // Calculate the sum of the two 3-bit numbers
        PORTC = sseg[sum]; // Display the sum on the seven-segment display
    }
    
    return (EXIT_SUCCESS); // Return Success
}
