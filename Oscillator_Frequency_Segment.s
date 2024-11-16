config OSC = INTIO2    ; Internal oscillator with port function on RA6 and RA7
config BOR = OFF       ; Brown-out Reset disabled
config STVREN = OFF    ; Stack overflow/underflow will not cause Reset
config WDT = OFF       ; Watchdog Timer disabled
config MCLRE = ON      ; MCLR Pin Enable bit is on

; RA0 - A, RA1 - B, RA2 - C, RA3- D, RA4 - E, RA5 - F, RA6 -G (output SSEG) / RD0 - #4 switch , RD1 - #3 switch (input Switchs)
; Using Common Anode for 7 Segment
   
#include <xc.inc>   ; Include device-specific definitions

#define _XTAL_FREQ 1000000  ; Define oscillator frequency

psect data		  ; Define program section for data
N       equ 0x50          ; Variable N to store the switch state
COUNTER equ 0x60          ; Counter for delay

psect code		  ; Define program section for code

start:	; Start label
    movlw   0x0f	    ; Load 0x0f into WREG
    movwf   ADCON1, a       ; Set all pins to digital I/O
    clrf    TRISA, a        ; Set PORTA as output
    movlw   0xff	    ; Load 0xff into WREG
    movwf   TRISD, a        ; Set PORTD as input

loop:	; loop label
    movf    PORTD, w, a     ; Move the value of PORTD to WREG
    andlw   0x03            ; Mask for RD0, RD1
    movwf   N, f, a         ; Sotre the masked value in N
    call    input	    ; Call the input subroutine
    goto    loop	    ; Repeat indifinitely

input:  ; input label
    ; Check if input is 0
    movf    N, w, a	     ; Move the value of N to WREG
    xorlw   0x00	     ; XOR WREG with 0x00
    bz      display_off	     ; Branch to display_off if zero

    ; Check if input is 1
    movf    N, w, a	     ; Move the value of N to WREG
    xorlw   0x01	     ; XOR WREG with 0x01
    bz      toggle_g	     ; Branch to toggle_g if zero

    ; Check if input is 2
    movf    N, w, a	     ; Move the value of N to WREG
    xorlw   0x02	     ; XOR WREG with 0x02
    bz      clockwise	     ; Branch to clockwise if zero

    ; Check if input is 3
    movf    N, w, a	     ; Move the value of N to WREG
    xorlw   0x03	     ; XOR WREG with 0x03
    bz      counterclock     ; Branch to counterclock if zero

    bra	    loop	     ; Branch to loop unconditionally

display_off:	; display_off label 
    setf    PORTA, a       ; Set all bits of PORTA (Turn off all SSEG) 
    call    delay500ms	   ; Call delay500ms subroutine
    bra	    loop	   ; Branch to loop

toggle_g:	; toggle_g label
    setf    PORTA, a	   ; Set all bits of PORTA (Turn off all SSEG)
    bcf     PORTA, 6, a    ; Turn on segment G 
    call    delay500ms	   ; Call delay500ms
    bsf     PORTA, 6, a    ; Turn off segment G 
    call    delay500ms	   ; Call delay500ms
    bra	    loop	   ; Branch to loop

clockwise:	; Rotate Clockwise label
    bsf     PORTA, 6, a    ; Ensure segment G is off
    
    ; Rotate segments clockwise
    bcf     PORTA, 0, a    ; Turn on segment A
    call    delay500ms	   ; Call delay500ms
    bsf     PORTA, 0, a    ; Turn off segment A 

    bcf     PORTA, 1, a    ; Turn on segment B 
    call    delay500ms	   ; Call delay500ms
    bsf     PORTA, 1, a    ; Turn off segment B 

    bcf     PORTA, 2, a    ; Turn on segment C 
    call    delay500ms	   ; Call delay500ms
    bsf     PORTA, 2, a    ; Turn off segment C 

    bcf     PORTA, 3, a    ; Turn on segment D 
    call    delay500ms	    ; Call delay500ms
    bsf     PORTA, 3, a    ; Turn off segment D 

    bcf     PORTA, 4, a    ; Turn on segment E 
    call    delay500ms	    ; Call delay500ms
    bsf     PORTA, 4, a    ; Turn off segment E 

    bcf     PORTA, 5, a    ; Turn on segment F 
    call    delay500ms	    ; Call delay500ms
    bsf     PORTA, 5, a    ; Turn off segment F 

    bra	    loop	    ; Branch to loop

counterclock:	    ; Rotate Counter Clockwise label
    bsf     PORTA, 6, a    ; Ensure segment G is off
    
    ; Rotate segments counter-clockwise
    bcf     PORTA, 5, a    ; Turn on segment F 
    call    delay500ms	    ; Call delay500ms
    bsf     PORTA, 5, a    ; Turn off segment F 

    bcf     PORTA, 4, a    ; Turn on segment E 
    call    delay500ms	    ; Call delay500ms
    bsf     PORTA, 4, a    ; Turn off segment E 

    bcf     PORTA, 3, a    ; Turn on segment D 
    call    delay500ms	    ; Call delay500ms
    bsf     PORTA, 3, a    ; Turn off segment D 

    bcf     PORTA, 2, a    ; Turn on segment C 
    call    delay500ms	    ; Call delay500ms
    bsf     PORTA, 2, a    ; Turn off segment C 

    bcf     PORTA, 1, a    ; Turn on segment B 
    call    delay500ms	    ; Call delay500ms
    bsf     PORTA, 1, a    ; Turn off segment B 
    
    bcf     PORTA, 0, a    ; Turn on segment A
    call    delay500ms	    ; Call delay500ms
    bsf     PORTA, 0, a    ; Turn off segment A 
    
    bra	    loop	    ; Branch to loop


delay2550us:	    ; Delay2550us label
    movlw   255	    ; Load WREG with 255
delay2550us_loop:  ; Delay 2550us loop label
    decfsz  WREG, f ; Decrement WREG and skip if zero
    goto    delay2550us_loop	; Repeat loop if WREG is not zero
    return  ; Return from subroutine

delay500ms:	; Delay 500ms label
    movlw   100		; Load WREG with 100
    movwf   COUNTER, f	; Move WREG to COUNTER
delay500ms_loop:    ; Delay 500ms loop label
    call    delay2550us	    ; Call delay2550us subroutine
    decfsz  COUNTER, f	    ; Decrement COUNTER and skip if zero
    goto    delay500ms_loop ; Repeat loop if COUNTER is not zero
    return		    ; Return from subroutine

end	    ; End of program
