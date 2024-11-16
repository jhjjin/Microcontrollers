config OSC = INTIO2     ; Oscillator (Internal oscillator block, port function on RA6 and RA7)   
config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)

; LED connected with 5V 
#include <xc.inc>	  ; Include a file xc.inc
    
    goto start		  ; Jump to the start label to begin execution
    psect code		  ; Define the start of the code section
    
    
start:	; Start label
    setf   ADCON1	   ;  AN inputs are digital inputs 
    bcf  TRISD, 1,  a	    ; Set RD1 as output
    bsf	  TRISB, 1, a	    ; Set RB1 as input

loop:  ; loop label
    btfss   PORTB, 1, a	    ; Check RB1 is high
    goto    ledOff	    ; If RB1 is low, jump to the ledOff label
    
    bcf	    PORTD, 1, a	    ; RB1 is high , LED turns on
    bra    loop		    ; Repeat the loop 

ledOff:	 ; LED turns off label
    
    bsf	   PORTD, 1 ,a	    ;  the LED turns off (RB1 is low)
    bra   loop		    ;  Repeat the loop
  
end 
    

