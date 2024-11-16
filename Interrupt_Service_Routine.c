
#include <stdio.h>      // Standard input-output library
#include <stdlib.h>     // Standard library
#include "config.h"     // Configuration header file

#define POTENTIOMETER  PORTBbits.RB1
#define LED PORTDbits.RD1

void __interrupt(high_priority) ISR(void) // Interrupt Service Routine
{
    if (INTCON3bits.INT1IF) // Check the INT1 External Interrupt flag bit is set or not
    {  
        if (POTENTIOMETER) // Check RB1 is high 
        {   
            LED = 0;  // Turn on LED
        } 
        else 
        {
            LED = 1;  // Turn off LED
        }
        INTCON3bits.INT1IF = 0;  // Clear TINT1 External Interrupt Flag bit
    }
}

int main() 
{
    ADCON1 = 0x0F;  // Set all pins as digital I/O

    TRISD1 = 0;     // Set RD1 as output
    TRISB1 = 1;     // Set RB1/INT1 as input 

    INTCON3bits.INT1IE = 1;  // Enables the INT1 external interrupt
    INTCON3bits.INT1IF = 0;  // Clear INT1 External interrupt flag bit
    INTCON3bits.INT1IP = 1;  // Set INT1 External interrupt high priority

    RCONbits.IPEN = 1;      // Enables priority Enable bit
    INTCONbits.GIEH = 1;    // Enables all high-priority interrupts
    INTCONbits.GIEL = 1;    // Enables all low-priority peripheral interrupts

    INTCON2bits.INTEDG1 = 1;    // Interrupt on rising edge
    PORTDbits.RD1 = 1;          // Turn off LED

    while (1) // Finish the main loop, waits for interrupts
    {
        
    }

    return 0;
}
