
#include <stdio.h>      // Standard input-output library
#include <stdlib.h>     // Standard library
#include "config.h"     // Configuration header file

#define POTENTIOMETER  PORTBbits.RB1       // Define POTENTIOMETER as RB1 bit of PORTB
#define LED PORTDbits.RD1                  // Define LED as RD1 bit of PORTD
void __interrupt(high_priority) ISR(void); // Prototype declaration for a high priority Interrupt Service Routine

int main() 
{
    ADCON1 = 0x0f;    // Set all pins as digital I/O
    TRISD = 0x00;     // Set RD1 as output
    TRISB = 0x0f;     // Set RB1/INT1 as input 
    
    INTCON3bits.INT1IE = 1;  // Enables the INT1 external interrupt
    INTCON3bits.INT1IF = 0;  // Clear INT1 External interrupt flag bit
    INTCON3bits.INT1IP = 1;  // Set INT1 External interrupt high priority
    
    RCONbits.IPEN = 1;      // Enable priority levels on interrupts
    INTCONbits.GIEH = 1;    // Enables high-priority interrupts
    INTCONbits.GIEL = 0;    // Disable low-priority peripheral interrupts

    INTCON2bits.INTEDG1 = 1;    // Interrupt on rising edge
    
    while (1) // Loop do nothing waiting for interrupt
    {
        
    }

    return (EXIT_SUCCESS);      // Return Success
}

void __interrupt(high_priority) ISR(void) // Interrupt Service Routine
{
    if (INTCON3bits.INT1IF) // Check the INT1 External Interrupt flag bit is set or not
    {  
        if (POTENTIOMETER == 1) // If the RB1 is high  
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
