#include <stdio.h>              // Standard input-output library
#include <stdlib.h>             // Standard library
#include "config.h"             // Configuration header file
#include "LiquidCrystal.h"      // LiquidCrystal header file 

#define _XTAL_FREQ 1000000      // Define the oscillator frequency 
volatile int counter;           // Declare a volatile itneger variable for the counter 

int main() 
{
    TRISA = 0x00;               // Set PORTA as output for LCD data
    TRISD = 0x00;               // Set PORTD as output for LCD control signals
    TRISB = 0x07;               // Set RB0, RB1, RB2 as input for push buttons, rest as output
    
    pin_setup(&PORTA, &PORTD);  // Set the PORTA as data port and the PORTD for signal port
    begin(16, 2, LCD_5x8DOTS);  // Set up the LCD with 16 columns, 2 rows, and 5x8 dot characters
    
    ADCON1 = 0x0f;              // Configure all port pins as digital I/O
    INTCON2bits.RBPU = 0;       // Enable internal pull-up resistors for PORTB
    counter = 0;                // Initialize the counter to 0
    
    ADCON0bits.GO = 1;          // Start the ADC conversion
    while(1)                                // Infinite loop
    {
        // Increment button (RB2)
        if (PORTBbits.RB2 == 0)             // Check if the increment button RB2 is pressed
        {
             while(PORTBbits.RB2 == 0)      // Continue while the button is pressed
             {
                    counter++;              // Increment the counter
                    home();                 // Move cursor to home position
                    print_int(counter);     // Print the current counter value
                    print("    ");          // Clear remaining Characters
             } 
        }
        
        // Decrement button (RB1)
        if (PORTBbits.RB1 == 0)             // Check if the increment button RB1 is pressed
        {
             while(PORTBbits.RB1 == 0)      // Continue while the button is pressed
             {
                    counter--;              // Decrement the counter
                    home();                 // Move cursor to home position
                    print_int(counter);     // Print the current counter value
                    print("    ");          // Clear remaining Characters
             }
        }

        // Reset button (RB0)
        if (PORTBbits.RB0 == 0)             // Check if the increment button RB0 is pressed
        {
            while(PORTBbits.RB0 == 0)       // Continue while the button is pressed
            {
                    counter = 0;            // Reset the counter to 0
                    home();                 // Move cursor to home postion
                    print_int(counter);     // Print the current counter value
                    print("    ");          // Clear remaining Characters
            }
        }
        ADCON0bits.GO =1;                   // Start the next ADC conversion
    }
    return 0;                               // Return Seccess
}
