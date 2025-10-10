# Microcontrollers_PIC18F (PIC18F4321)
```
A collection of hardware-verified embedded C and assembly projects on PIC18F4321.
Built with MPLAB X + XC8, programmed and debugged on real hardware using Microchip MPLAB PICkit 4.
Covers LCD (HD44780), ADC, PWM, external & timer interrupts, seven-segment displays, switches/LEDs — a full sensor → compute → act/display pipeline.
- Verified on physical circuits with PICkit 4
- Both C and ASM implementations
- Clean modularization (custom LCD driver + focused examples)
```
---
## Development Environment
```
- MCU: PIC18F4321
- Toolchain: MPLAB X IDE, XC8
- Programmer/Debugger: Microchip MPLAB PICkit 4 (ICSP/In-Circuit Debugging)
- Clock: Internal oscillator (examples assume _XTAL_FREQ = 1 MHz)
- Display: HD44780 16×2 (8-bit mode)
- I/O: Common-anode 7-segment, push buttons (PORTB pull-ups), potentiometers on AN0/AN1
```
---
## Hardware Pin Mapping (Common Pattern)
```
- LCD (8-bit)
    Data (D0–D7): PORTA or PORTC
    Control: RD0=RS, RD1=RW, RD2=E
    Usage: pin_setup(&PORTx, &PORTD); → begin(16, 2, LCD_5x8DOTS);
- Switches: PORTB (enable pull-ups with INTCON2bits.RBPU = 0)
- 7-Segment (Common Anode): Segment ON = LOW
- ADC: AN0/AN1 as potentiometer inputs
```
---
## Module & Example Index (What Each Demo Shows)

## LCD Driver (Custom)
```
LiquidCrystal.c / LiquidCrystal.h
Full 8-bit driver for HD44780: pin_setup, begin, print/print_int, setCursor, clear/home, cursor/blink/scroll, and low-level sendData/sendCommand/pulseEnable/write8bits.
→ Demonstrates driver-level control without Arduino libraries.
```

## ADC + ISR + Math + LCD
```
- ADC_Conversion_Interrupt_Priority.c (+config)
  AN0 10-bit → voltage computation (integer/fraction) → LCD; also mirrors to 7-seg nibbles. Uses ADC interrupt.
- ADC_Interrupt_Service_Routine_Case.c (+config)
  Dual ADC (AN0/AN1) mapped to signed range (−512..+511). Switch selects Add/Sub/Mul; prints expression & result on LCD.
```
## Buttons / Counter / Internal Pull-ups
```
- Button-Controlled LCD Counter.c
  RB2 increment, RB1 decrement, RB0 reset — live LCD updates.
- Internal_Pull-Up_Resistors_PORTB_Cursor.c (+config)
  PORTB pull-ups enabled; clean LCD output/cursor handling.
```

## External Interrupts (INT1) / Priority
```
- High_Priority_Interrupt_Service_Routine_LED.c (+config)
  INT1 (RB1) rising-edge interrupt toggles RD1 LED. Shows priority, edge, and flag handling.
- Interrupt_Service_Routine.c (+config)
  Alternate C version (GIEH/GIEL, IPEN) for comparison.
- High_Priority_Interrupt_Service_Routine_LED.s, Interrupt_Service_Routine.s
  Assembly ISR implementations: vectoring, stack, retfie 0.
```
## Seven-Segment & Switch Logic
```
- Read_Lower_Four_Bits.c / .s (+config)
  RB0..RB3 → BCD → 7-seg lookup (C & ASM).
- Read_First&Second_3-bit Numbers_Sum.c / .s (+config)
  Two 3-bit numbers (RD0..RD2 and RD3..RD5) → sum → 7-seg.
  ASM shows TBLRD+* program-to-data table copy & PLUSW0 indexed access.
- Oscillator_Frequency_Segment.c / .s (+config)
  RD0/RD1 choose modes: all off / toggle G / CW / CCW segment rotation. ASM includes compact software delay routines.
```
## PWM / Timers / Speed Control
```
- Setup_Timer2_FOSC.c (+config)
  Timer2 + CCP2 PWM @1 kHz, ADC ISR sets duty cycle (CCPR2L high 8 bits, DC2B low 2 bits).
- PIC18F Microcontroller: ADC-based PWM Speed Controller.c
  Integrated example: AN0 = max duty, AN1 = delay(ms), INT0 toggles Run/Stop, Timer0 low-priority tick, LCD shows speed/delay, LEDs indicate state.
  → End-to-end sensor → control → UI system.
```
## Simple Port Mirror / Toggle
```
- LED ON_OFF.c / LED ON_OFF .s (+config)
  RC1/RD1 switches control RC0/RD0 LEDs (common anode logic).
- Potentiometer_PPORTD_PORTB.c / .s (+config)
  PORTB input → inverted (~) → PORTD output.
```
## Note 
```
- Real hardware validation using MPLAB PICkit 4 (not just simulation).
- Complete embedded pipeline: ADC acquisition → ISR/compute → PWM/LED/LCD output with responsive UI.
- Comfortable in C and Assembly: bit/flag handling, interrupt vectors & priority, timing, program/data memory tables.

```
