   #include <xc.inc>
    ;psect code
    
    num_AH  equ	0x20
    num_AM  equ 0x21
    num_AL  equ 0x22
    num_BH  equ 0x50
    num_BM  equ	0x51	
    num_BL  equ	0x52
    num_CH  equ	num_BH	
    num_CM  equ	num_BM
    num_CL  equ	num_BL
  
    ; A = F1 91 B5
    movlw 0xF1
    movwf num_AH, f, a
    movlw 0x91
    movwf num_AM,f, a
    movlw 0xB5
    movwf num_AL, f, a
    
    ; B = 07 A2 04
    movlw 0x07
    movwf num_BH, f, a
    movlw 0xA2
    movwf num_BM, f, a
    movlw 0x04
    movwf num_BL, f, a
    
    clrf WREG, a 
    
    ;sum low bytes
    movwf num_AL, w, a
    addwf num_BL, w, a
    
    movwf num_CL, f, a
    
    ; sum mid bytes
    movwf num_AM, w, a
    addwfc num_BM, w, a
    
    movwf num_CM, f, a
    
    ; sum High bytes
    movwf num_AH,w, a 
    addwfc num_BH, w, a
    
    movwf num_CH, f, a
    
            
end




