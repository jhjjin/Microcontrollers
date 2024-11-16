config OSC = INTIO2 ; Oscillator (Internal oscillator block, port function on RA6 and RA7)
config BOR = OFF   ;   Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config WDT = OFF    ;   Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON    ;  MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
config STVREN = OFF  ;  Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)

; LED connected with 5V which mean common anode - 0 (low) LED ON
#include <xc.inc>   ; Include a file xc.inc

    goto start ; Jump to the start label to begin execution
    psect code  ; Define the start of the code section 
start:	; start label
    movlw   0x0F    ; load the value 0x0f into WREG
    movwf   ADCON1  ; Move the value in the WERG to ADCON1 Reg to set pins as digital

    ; RC0, RD0 are LED outputs: 0
    ; RC1, RD1 are switch inputs: 1
    clrf    TRISC, a      ; Clear the TRISC register to set pins on PORTC as initially
    clrf    TRISD, a       ; Clear the TRISD register to set pins on PORTD as initially 
    bsf     TRISC, 1, a    ; RC1 as input
    bsf     TRISD, 1, a    ; RD1 as input

infloop:    ; infiteloop label
    ; Check RC1 switch, control RC0 LED
    btfsc   PORTC, 1, a    ; Skip next line if RC1 is low
    bcf     PORTC, 0, a   ; Turn Off LED at RC0 if switch not pressed
    btfss   PORTC, 1, a    ; Skip next line if RC1 is high
    bsf     PORTC, 0, a    ; Turn On LED at RC0 

    ; Check RD1 switch, control RD0 LED
    btfsc   PORTD, 1, a    ; Skip next line if RD1 is low
    bcf     PORTD, 0, a    ; Turn Off LED at RD0 
    btfss   PORTD, 1, a    ; Skip next line if RD1 is high
    bsf     PORTD, 0, a    ; Turn On LED at RD0 
    
    goto    infloop	    ; Repeat the loop infinitely

    end			    ; End program
