#include <stdio.h>              // Standard input-output library
#include <stdlib.h>             // Standard library
#include "config.h"             // Configuration header file
#include "LiquidCrystal.h"      // LiquidCrystal header file 

#define _XTAL_FREQ 1000000      // Define the oscillator frequency for delay function

// Data pins connected to PORTC, Signal pins connected to PORTD (RD0 - RS, RD1 - RW, RD2 - E)
void __interrupt() adc(void);   // Prototype for the ADC interrupt service routine
volatile int num;               // The variable store the ADC result 
volatile float voltage;         // The variable store the calculated voltage values
volatile int whole;             // The variable store the whole number part of the voltage
volatile int fractional;        // The variable store the fractional part of the voltage

int main() 
{
    TRISB = 0x00;               // Set PORTB as output (7 segment)
    TRISC = 0x00;               // Set PORTC as output
    TRISD = 0x00;               // Set PORTD as output 
    
    pin_setup(&PORTC, &PORTD);  // Set the PORTC as data port and the PORTD for signal port
    begin(16, 2, LCD_5x8DOTS);  // Set up the LCD with 16 columns, 2 rows, and 5x8 dot characters

    ADCON1 = 0x0E;          // Configure AN0 as analog input    
    ADCON2bits.ADCS = 0;    // ADC conversion clock select (Fosc/2)
    ADCON2bits.ACQT = 1;    // Acquisition time select (2 TAD)
    ADCON2bits.ADFM = 1;    // ADC result format select (right justified)
     
    ADCON0bits.CHS = 0;     // Select AN0 as ADC input channel
    ADCON0bits.ADON = 1;    // A/D converter module is enable
    
    PIR1bits.ADIF = 0;      // Clear the ADC interrupt flag
    PIE1bits.ADIE = 1;      // Enable the ADC interrupt
    IPR1bits.ADIP = 1;      // Set the ADC interrupt priority
    RCONbits.IPEN = 0;      // Disable interrupt priority levels
    INTCONbits.PEIE = 1;    // Enable peripheral interrupts
    INTCONbits.GIE = 1;     // Enable global interrupts
    
    ADCON0bits.GO = 1;      // Start the ADC conversion

    while(1)                                         // Infinite loop
    {
        voltage = (num * 5.0) / 1023.0;              // Convert to ADC value to a voltage 
        whole =  voltage;                            // Assign the whole number part of the voltage
        fractional = ((voltage - whole) * 10);       // Assign the fractional part of the voltage
        
        home();                                      // Return cursor position to home
        print("Row: ");                              // Print "Row: " on the LCD
        print_int(num);                              // Print the ADC values on the LCD 
        print("      ");                             // Clear remaining character
        PORTB = (whole << 4) | (fractional & 0x0f);  // Using left shift operator shifts the bits of whole to left and send the data to 7 segment

        __delay_ms(30);    
        ADCON0bits.GO = 1;                           // Start the next ADC conversion
    }

    return 0;                                        // Return Success
}

void __interrupt() adc(void)                        // ADC interrupt service routine
{   
    if(PIR1bits.ADIF && PIE1bits.ADIE)              // Check if the ADC interrupt occured
    {
        PIR1bits.ADIF =0;                           // Clear the ADC interrupt flag
        num = (ADRESH << 8) | ADRESL;               // Combine the high and low byes of the ADC values
        ADCON0bits.GO = 1;                          // Start the next ADC conversion
    }
}