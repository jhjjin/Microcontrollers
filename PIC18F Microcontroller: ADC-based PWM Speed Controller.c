#include <stdio.h>          // Standard input-output library
#include <stdlib.h>         // Standard library
#include "config.h"         // Configuration header file
#include "LiquidCrystal.h"  // LiquidCrystal header file 

#define _XTAL_FREQ 1000000      // Define the oscillator frequecny

#define GREEN_LED PORTCbits.RC1 // Define the Green Led pin connected to RC1
#define RED_LED PORTBbits.RB1   // Define the Red Led pin connected to RB1
#define SWITCH  PORTBbits.RB0   // Define the switch pin connected to RB0 

volatile unsigned int speed;             // Store the ADC result from AN0 
volatile unsigned int delay;             // Store the calculated delay from AN1
volatile unsigned int delay_time;        // Store the calculated delay_time   
volatile unsigned int position = 0;     // Based on RB0 Switch -> 0 = stopped, 1 = running

// Function declarations 
void delay_ms(unsigned int ms);                 // Prototype for the delay_ms
void toggle_state();                            // Prototype for the toggle_state 
void update_pwm(unsigned int target_duty);      // Prototype for the upadted PWM
void __interrupt(high_priority) high_isr(void); // Prototype for the high interrupt service routine
void __interrupt(low_priority) low_isr(void);   // Prototype for the low interrupt service routine

int main() 
{
    // Configure ADC
    ADCON1 = 0x0D;                  // Configure AN0 and AN1 as input
   
    ADCON2bits.ADFM = 1;            // Right justify the result
    ADCON2bits.ACQT = 1;            // ADC Acquisition time 2 TAD
    ADCON2bits.ADCS = 0;            // ADC conversion clock Fosc / 2
    
    ADCON0bits.CHS = 0;             // Analog Channel 0 selected 
    ADCON0bits.ADON = 1;            // A/D converter module is enabled
    PIE1bits.ADIE = 1;              // Enable ADC interrupt
    PIR1bits.ADIF = 0;              // Clear ADC interrupt flag
    IPR1bits.ADIP = 0;              // Set ADC interrupt as low-priority
    INTCONbits.GIE = 1;             // Enable all interrupts
    
    // Configure LCD
    TRISD = 0x00;                   // Set PORTD as output 
    TRISE = 0x00;                   // Set PORTE as output
    pin_setup(&PORTD, &PORTE);      // Set the PORTD as data port PORTE as signal port
    begin(16, 2, LCD_5x8DOTS);      // LCD 16x2 size, 5x8 dots per character

    // Configure LEDs and Switch
    TRISCbits.RC1 = 0;              // Set RC1 as output for GREEN LED
    TRISBbits.RB1 = 0;              // Set RB1 as output for RED LED
    TRISBbits.RB0 = 1;              // Set RB0 as input for SWITCH

    // Setup PWM for CCP2
    PR2 = 249;                      // Set PWM period for 1 kHz frequency 
    T2CONbits.TMR2ON = 1;           // Timer 2 is on
    T2CONbits.T2CKPS= 0x11;         // Set prescaler is 16
    CCP2CONbits.CCP2M = 0b1100;     // Set CCP2 module PWM mode
    //CCPR2L = 0x00;                  // Initial duty cycle 0%

    // Setup Timer0
    T0CONbits.TMR0ON = 1;           // Enable Timer0
    T0CONbits.PSA = 0;              // Timer0 prescaler is assigned. 
    T0CONbits.T0PS = 0x02;          // Timer0 Prescaler 1:8 
    T0CONbits.T0CS = 0;             // Set internal instruction cycle clock 
    T0CONbits.T08BIT = 0;           // SEt Timer 0 to 16 bit mode
    
    //TMR0 = 0;                     // Load Timer0 to overflow in 10ms

    // Setup interrupt
    RCONbits.IPEN = 1;               // Enable interrupt priority levels
    INTCON2bits.INTEDG0 = 0;         // Interrupt on falling edge for RB0
    INTCONbits.INT0IE = 1;           // Enable the INT0 exteral interrupt
    INTCONbits.INT0IF = 0;           // Clear INT0 interrupt flag

    INTCONbits.TMR0IE = 1;          // Enable Timer0 overflow interrupt
    INTCONbits.TMR0IF = 0;          // Clear Timer0 interrupt flag
    INTCON2bits.TMR0IP = 0;         // Set Timer0 as low-priority interrupt
    position = 1;                   // Running the circuit initially
    RED_LED = 0;                    // Red LED off initially
    GREEN_LED = 0;                  // Green LED off initially
    
    while(1)                        // Infinite loop
    {
        if (position == 1)                    // If the circuit is running
        {
            
            // Read ADC values
            ADCON0bits.CHS = 0;               // Select AN0 
            ADCON0bits.GO = 1;                // Start ADC conversion
            while (ADCON0bits.GO);            // Wait for conversion to complete
            speed = (ADRESH << 8) | ADRESL;   // Store ADC reulst in the speed

            ADCON0bits.CHS = 1;               // Switch to AN1
            ADCON0bits.GO = 1;                // Start ADC conversion
            while (ADCON0bits.GO);            // Wait for conversion to complete
            delay = (ADRESH << 8) | ADRESL;   // Store ADC reuslt in the delay
            delay_time = delay * 10;
            // Display on LCD
            home();                           // Set cursor to the home position
            print("Max Speed: ");             // Print "Max Speed: on LCD
            print_int(speed);                 // Print the speed value
            print("    ");                    // Clear remaining characters in the row 
            setCursor(0, 1);                  // Move cursor to the second row
            print("Delay: ");                 // Print "Delay: 
            print_int(delay_time);            // Print the delay time 
            print(" ms");                     // print "ms" 
            print("     ");                   // Clear remaining characters in the row
            RED_LED = 0;                      // Turn off the RED_LED

            // Increasing the PWM duty cycle
            for (unsigned int i = 0; i <= speed; i++) 
            {
                update_pwm(i);                 // update the PWM duty cycle
                __delay_ms(1);                 // Adjust speed change rate here
            }
            delay_ms(delay_time);              // Delay after reaching max speed
        }
       else
        {
            RED_LED = 1;                        // Turn on RED LED
            GREEN_LED = 0;                      // Turn off GREEN LED
            home();                             // Set cursor to the home position
            print("   STOPPED!   ");            // Print "STOPPED!"
            setCursor(0, 1);                    // Move cursor to the second row
            print("                  ");        // Clear second line
        }
    } 
    return 0;                                   // Return Success
}
void delay_ms(unsigned int ms)
{
    while(ms-- > 0)                             // repeatedly in a loop until ms is greater than 0
    {
        __delay_ms(1);                          // delay 1ms 
    }
}
void update_pwm(unsigned int target_duty) 
{
    if(position == 1)                           // If the circuit is running 
    {
        CCPR2L = target_duty / 4;               // Set the upper 8 bits of the duty cycle
        CCP2CONbits.DC2B = target_duty & 0x03;  // Set the lower 2 bits of the duty cycle
    } 
    else 
    {
        CCPR2L = 0;                             // Turn off PWM output
    }
}

void toggle_state() 
{
    position = !position;                       // Toggle the state
    if (position == 0)                          // If the circuit is stopped
    {
        CCPR2L = 0;                             // Turn off PWM output
        RED_LED = 1;                            // Turn on RED LED
        GREEN_LED = 0;                          // Turn off GREEN LED
        
    } 
    else if (position == 1)                     // If the circuit is running
    {
        RED_LED = 0;                            // Turn off RED LED
        GREEN_LED = 1;                          // Turn on GREEN LED
    }
}

// High-priority ISR for INT0 (toggle state)
void __interrupt(high_priority) high_isr(void) 
{
    if (INTCONbits.INT0IF)     // Check if INT0 interrupt occurred
    {  
        INTCONbits.INT0IF = 0;                      // Clear the interrupt flag
        toggle_state();                             // Toggle system state on button press
    }
}

// Low-priority ISR for Timer0 and ADC
void __interrupt(low_priority) low_isr(void) 
{
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF)      // check if Timer 0 interrupt occured
    {  
        INTCONbits.TMR0IF = 0;                       // Clear Timer0 interrupt flag
        TMR0 = 65224;                                // Load Timer 0 as 10ms
    }

}
