

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define LED PORTCbits.RC1

void __interrupt() ISR(void);

int main() 
{
    PR2 = 249;      // PWM frequency = 1KHz
    
    // Setup timer 2 , FOSC = 1MHz
    T2CONbits.T2CKPS = 0b00;
    T2CONbits.TMR2ON = 1;
    
    
    
    TRISCbits.RC1 = 0;
    CCP2CONbits.CCP2M = 0b1100;
    
    ADCON1 = 0x0E;
    TRISAbits.RA0 = 1;
    
    ADCON2bits.ADCS = 0;
    ADCON2bits.ACQT = 1;
    ADCON2bits.ADFM = 0;
    
    ADCON0bits.CHS = 0;
    ADCON0bits.ADON = 1;
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 1;
    
    RCONbits.IPEN = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    while(1)
    {
        ADCON0bits.GO = 1;
    }
    
    
    return 0;
}

void __interrupt() ISR(void)
{
    if(PIR1bits.ADIF && PIE1bits.ADIE)
    {
        PIR1bits.ADIF = 0;
        CCPR2L = ADRESH;
        CCP2CONbits.DC2B = ((ADRESL >> 6)& 0x03);
        
    }
}

