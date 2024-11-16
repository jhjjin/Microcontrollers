config OSC = INTIO2     ; Set Oscillator to internal with RA6/ RA7 as port function
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#include <xc.inc>       ; Include a file xc.inc

    goto start ; Jump to the start label to begin execution

; PORTD 0~6 is input for switches
; RC0 - A, RC1 - B, RC2 - C , RC3 - D, RC4 - E, RC5 - F, RC6 - G
; Using Common Anode for 7 segment 
    
    psect data  ; Define program section for data
lookup: DB 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86 ; 7 segment patterns for 0-9, A-E

    psect code  ; Define program section for code
SSEG equ 0x100    ; 7 segment pattern table starting address in data memory
I equ 0x70       ; Used as a counter index 
firstNumber equ 0x60	; Define firstNumber at memory location 0x60
secondNumber equ 0x61	; Define secondNumber at memory location 0x61
SUM equ 0x62		; Define SUM at memory location 0x62

start:	    ; Start label
    movlw 0xff		 ; Load the 0xff into the WREG
    movwf ADCON1, a      ; Set all pins as digital I/O
    clrf TRISC, a        ; Set PORTC as output
    movlw 0xff		 ; Load the 0xff into the WREG
    movwf TRISD, a       ; Set PORTD as input
    movlw 0xff		 ; Load the 0xff into the WREG
    movwf PORTC, a       ; Initialize PORTC to 0xff which is turning off all segments

    movlw low lookup	       ; Move low byte of lookup address to WREG
    movwf TBLPTRL, f, a	       ; Move WREG content to table pointer low register
    movlw high lookup	       ; Move high byte of lookup address to WREG
    movwf TBLPTRH, f, a	       ; Move WREG content to table pointer high register
    movlw low highword lookup  ; Move low byte of the high word of lookup address to WREG
    movwf TBLPTRU, f, a	        ; Move WREG content to table pointer upper register
    
    lfsr 0, SSEG         ; Starting address in data memory
    movlw 15             ; Load WREG with 15 
    movwf I, f, a        ; Initialize counter with 15
loop:
    TBLRD*+              ; Read 1B from program memory and advance TBLPTR by 1
    movff TABLAT, POSTINC0 ; Copy TABLAT into INDF0 then move FSR0 pointer forward
    decf I, f, a         ; Decrement I register 
    bnz loop             ; If I is not zero, loop back
    
infiniteloop:
    ; Read the first 3-bit number from switches 0(4), 1(2), and 2(1) 
    clrf firstNumber, a	   ; Clear the firstNumber 
    btfsc PORTD, 0, a	   ; Test bit 0 of PORTD, skip next if clear
    bsf firstNumber, 2, a ; Set bit 2 (4)
    btfsc PORTD, 1, a	   ; Test bit 1 of PORTD, skip next if clear
    bsf firstNumber, 1, a ; Set bit 1 (2)
    btfsc PORTD, 2, a	   ; Test bit 2 of PORTD, skip next if clear
    bsf firstNumber, 0, a ; Set bit 0 (1)

    ; Read the second 3-bit number from switches 3(4), 4(2), and 5(1) 
    clrf secondNumber, a   ; Clear the secondNumber
    btfsc PORTD, 3, a	   ; Test bit 3 of PORTD, skip next if clear
    bsf secondNumber, 2, a ; Set bit 2 (4)
    btfsc PORTD, 4, a	   ; Test bit 4 of PORTD, skip next if clear
    bsf secondNumber, 1, a ; Set bit 1 (2)
    btfsc PORTD, 5, a	   ; Test bit 5 of PORTD, skip next if clear
    bsf secondNumber, 0, a ; Set bit 0 (1)

    ; Calculate the sum of the firstNumber and secondNumber
    movf firstNumber, w, a	; Move firstNumber to WREG
    addwf secondNumber, w, a	; Add WREG to secondNumber, store in WREG
    movwf SUM, a		; Move the SUM from WREG to SUM
    
   
    ; Convert sum to 7 segment 
    movf SUM, w, a	; Move SUM to WREG
    call bcd2sseg	; Call subroutine to 7 segment
    movwf PORTC, a      ; Write the converted pattern to PORTC

    bra infiniteloop    ; Repeat indefinitely
    
bcd2sseg:		; bcd to 7 segment
    lfsr 0, SSEG        ; Set FSR0 to start of SSEG
    movf PLUSW0, w, a   ; Read the 7 segment pattern from data memory 
    return 0            ; Return with pattern in WREG

end                     ; End program
