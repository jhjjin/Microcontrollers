config OSC = INTIO2	; Set Oscillator to internal with RA6/ RA7 as port function
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>	; include device-specific definitions
	goto start	; Jump to start of prgram
		
	psect data	; Define program section for data
lookup:	DB  0x81, 0xCF, 0x92, 0x86, 0xCC, 0xA4, 0xA0, 0x8F, 0x80, 0x84, 0x88, 0x80, 0xB1, 0xC2, 0xB0, 0xB8 ; 7 segment patterns for 0-9, A-F
	
	psect code	; Define program section for code
SSEG	EQU 0x100    ; 7segment pattern table starting address in data memory
I	EQU 0x70    ; used as a counter index
start:	
	; Move the 7seg pattern from program memory into data memory
	movlw	low lookup	    ; Move low byte of lookup address to WREG
	movwf	TBLPTRL, f, a	    ; Move WREG content to table pointor low register
	movlw	high lookup	    ; Move high byte of lookup address to WREG
	movwf	TBLPTRH, f, a	    ; Move WREG content to Table pointer high register
	movlw	low highword lookup ; Move low byte of the high word of lookup address to WREG
	movwf	TBLPTRU, f, a	    ; Move WREG content to table pointer upper register
	
	lfsr	0, SSEG ; starting address in data memory
	movlw	16	; Load WREG with 16 
	movwf	I, f, a ; initialize counter with 10
loop:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I, f, a;    ; Decreament I register 
	bnz	loop	    ; If I is not zero, loop back
	
	
	; set the I/O port directions
	setf	ADCON1, a   ; Configure all pins as digital I/O by setting ADCON1
	clrf	TRISA, a    ; Set pins of PORTA as output
	setf	TRISB, a    ; Set pins of PORTB as input
	
	; start the read/display
infiniteloop:
	movf	PORTB, w, a ; Read PORTB into WREG
	andlw	0x0f	    ;  Mask lower 4 bits
	call	bcd2sseg, 0 ; Call subroutine to convert to 7 segment pattern
	movwf	PORTA, a    ; Write the converted partten to PORTA
	bra	infiniteloop	; Repeat indefinitely
    
	; convert a BCD pattern stored in 4 lsb of WREG into 7Seg
bcd2sseg: 
	lfsr	0, SSEG	    ; Set FSR0 to start of SSEG
	movf	PLUSW0, w, a ; Read the 7 segment pattern from data memory 
	return 0	    ; Return with pattern in WREG
	
end			    ; End program


