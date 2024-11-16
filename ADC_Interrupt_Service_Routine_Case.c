#include <stdio.h>              // Standard input-output library
#include <stdlib.h>             // Standard library
#include "config.h"             // Configuration header file
#include "LiquidCrystal.h"      // LiquidCrystal header file 

#define _XTAL_FREQ 1000000      // Define the oscillator frequency
void __interrupt() adc(void);   // Prototype for the ADC interrupt service routine
volatile int firstNum, secondNum, result;   // Declare a volatile itneger variable for the firstNum, secondNum, result
volatile int position;          // Declare a volatile integer variable for the position
volatile int valueAdc;          // Declare a volatile integer variable for the value of ADC
volatile int valueSign;         // Declare a volatile integer variable for the signed value

int main() 
{
    TRISA = 0x03;               // Set RA0 and RA1 as input for potentiometers (AN0 and AN1)
    TRISB = 0x00;               // Set PORTB as output for LCD data
    TRISD = 0x18;               // Set RD3 and RD4 as input for switches, RD0-RD2 as output for LCD control

    pin_setup(&PORTB, &PORTD);  // Set the PORTB as data port and the PORTD for signal port
    begin(16, 2, LCD_5x8DOTS);  // Set up the LCD with 16 columns, 2 rows, and 5x8 dot characters

    ADCON1 = 0x0D;              // Configure AN0 and AN1 as analog inputs
    ADCON2bits.ADCS = 0;        // ADC conversion clock select (Fosc/2)
    ADCON2bits.ACQT = 1;        // Acquisition time select (2 TAD)
    ADCON2bits.ADFM = 1;        // ADC result format select (right justified)
     
    ADCON0bits.CHS = 0;         // Select AN0 as ADC input channel
    ADCON0bits.ADON = 1;        // A/D converter module is enable
    
    PIR1bits.ADIF = 0;          // Clear the ADC interrupt flag
    PIE1bits.ADIE = 1;          // Enable the ADC interrupt
    IPR1bits.ADIP = 1;          // Set the ADC interrupt priority
    RCONbits.IPEN = 0;          // Disable interrupt priority levels
    INTCONbits.PEIE = 1;        // Enable peripheral interrupts
    INTCONbits.GIE = 1;         // Enable global interrupts
    ADCON0bits.GO = 1;          // Start the ADC conversion

    while (1)                                           // Infinite Loop
    {
            position = (PORTD >> 3) & 0x03;             // Shift right three position and PORTD bits to get switch position
            if (position == 0)                          // If the switch position is 0  (Display is off)
            {
                displayOFF();                           // Turn off display
                continue;                               // Skip the rest of the loop
            }
             
            switch (position)                           // Check the switch position 
            {
                case 1:                                 // Case 1: the switch position is 1 (Add the two signed integers)
                    displayON();                        // Turn on the display 
                    result = firstNum + secondNum;      // Add the first numbers and second numbers and assign to result                    
                    home();                             // Return cursor position to home
                    print("(");                         // Print (
                    print_int(firstNum);                // Print first numbers
                    print(")+(");                       // Print )+(
                    print_int(secondNum);               // Print second numbers
                    print(")=");                        // Print )= 
                    print("      ");                    // Clear remining character
                    setCursor(0, 1);                    // Move to the second line
                    print_int(result);                  // Print result 
                    print("      ");                    // Clear remaining character
                    break;                              // End processing of statement
                    
                case 2:                                 // Case 2: the switch position is 2 (Subtract the two signed integers)
                    displayON();                        // Turn on the display
                    result = firstNum - secondNum;      // Subtract the first numbers and second numbers and assign to result
                    home();                             // Return cursor position to home
                    print("(");                         // Print (
                    print_int(firstNum);                // Print first numbers
                    print(")-(");                       // Print )-(
                    print_int(secondNum);               // Print second numbers
                    print(")=");                        // Print )= 
                    print("      ");                    // Clear remaining character
                    setCursor(0, 1);                    // Move to the second line
                    print_int(result);                  // Print result 
                    print("      ");                    // Clear remaining character                   
                    break;                              // End processing of statement
                    
                case 3:                                 // Case 3: the switch position is 3 (Multiply the two signed integers)
                    displayON();                        // Turn on the display
                    result = firstNum * secondNum;      // Multiply the first numbers and second numbers and assign to result                  
                    home();                             // Return cursor position to home
                    print("(");                         // Print (
                    print_int(firstNum);                // Print first Number 
                    print(")x(");                       // Print )x(
                    print_int(secondNum);               // Print second numbers
                    print(")=");                        // Print )= 
                    print("      ");                    // Clear remining character
                    setCursor(0, 1);                    // Move to the second line
                    print_int(result);                  // Print result 
                    print("      ");                    // Clear remining character
                    break;                              // End processing of statement
                default:                                // default case 
                    break;                              // End processing of statement
                    
                ADCON0bits.GO = 1;                      // Start the next ADC conversion
            }        
    }
    return 0;                                           // Return Success
}

void __interrupt() adc(void)                             // Interrupt service routine to read ADC values
{
    if (PIR1bits.ADIF&PIE1bits.ADIE)                     // Check if ADC interrupt flag is set
    {
            PIR1bits.ADIF = 0;                           // Clear ADC interrupt flag
            valueAdc = (ADRESH << 8) | ADRESL;           // Read 10 bit ADC values 
            valueSign = valueAdc - 512;                  // Convert to signed value from 0~1023 to -512 ~511
            
            if (ADCON0bits.CHS == 0)                     // Amalog Channel Select bits 0 (AN0))
            {
                firstNum = valueSign;                    // Adjust first number range to -512 ~ 511
                ADCON0bits.CHS = 1;                      // Switch to next channel AN1
            } 
            else                                         
            {
                secondNum = valueSign;                   // Adjust second number range to -512 ~ 511
                ADCON0bits.CHS = 0;                      // Switch back to AN0
            }
            ADCON0bits.GO = 1;                           // Start next ADC conversion     
    }
}