config OSC = INTIO2     ; Oscillator (Internal oscillator block, port function on RA6 and RA7)
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)d
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)

; LED connected with 5V
#include <xc.inc>	; Include a file xc.inc
	goto main	; Jump to main program 
psect intCodeHi, class = CODE, reloc = 2    ; Define high priority interrupt code section
MAIN equ 0x80           ; Define the start address of the main program

 ;ISR 
 call ISR   ; Call ISR
 retfie 0   ; Return from interrupt 

main:	; main label 
    movlw 0x0f		; Set all pins as digital I/O
    movwf ADCON1, a	; Set ADCON1 to 0x0f to pins as digital I/O
    
    bcf TRISD, 1, a     ; Clear to set RD1 as output
    bsf TRISB, 1, a     ; Set to set RB1 as input

    movlw 0x10		; Load the 0x10 into the WREG
    movwf STKPTR, a	; Initialize stack pointer to 0x10
        
    bcf INTCON3, 0, a   ; Clear INT1 interrupt flag bit 
    bsf INTCON3, 6, a   ; Set INT1 interrupt to high priority
    bsf	INTCON3, 3, a   ; Enable the INT1 external interrupt
    
    bsf RCON, 7, a       ; Enable priority levels on interrupts
    bsf INTCON, 7, a     ; Enable high priority interrupts
    bcf INTCON, 6, a     ; Disable low priority peripheral interrupts
    
    bsf INTCON2, 5, a   ; Interrupt on rising edge
        
loop:	; loop label 
    goto  loop     ; Loop do nothing waiting for interrupt
       
ISR:	; ISR label 
     btfss INTCON3, 0 , a    ; bit test bit 0 of INTCON3, skip if set
     retfie		     ; Return from interrupt if flag is not set
     
     btfsc PORTB, 1, a	     ; Test bit 1 of PORTB, skip next if clear
     bcf PORTD, 1, a	     ; Turn on the LED
     btfss PORTB, 1, a	     ; Test bit 1 of PORTB, skip next if set
     bsf PORTD, 1, a         ; Turn off the LED 
    
    bcf INTCON3, 0, a	     ; Clear INT1 interrupt flag
    retfie 0		     ; Return from interrupt 

end
 
	