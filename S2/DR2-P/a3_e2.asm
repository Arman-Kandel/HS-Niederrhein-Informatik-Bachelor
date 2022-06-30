; START:
; Setup von Addressen, zum
; kontrollieren der Schalter und LEDs.
;
; LOOP:
; Schalter 0, 1, 2, 3 werden gelesen
; und bits auf LEDs 0, 1, 2, 3 übertragen.
;
; Falls Schalter 7 = true, werden die
; bits invertiert, befor sie auf die LEDs übertragen werden.
ABSENTRY  MAIN
       ORG   $1000 
MAIN   LDS   #4000 ; load stackpointer to last address

; Define ports
PORTA  EQU   $00
PORTB  EQU   $01
DDRA   EQU   $02
DDRB   EQU   $03
; Set PA7,PA6,PA5,PA4 to (1)OUTPUT (Ports zu LED Anzeige (D0-D3))
; and PA3,PA2,PA1,PA0 to (0)INPUT (Schalter (S0-S3) zu Ports)
       MOVB #%11110000,DDRA
; Set PB7...PB0 INPUT (special INPUT (Schalter, S7) port B: PB7)
       MOVB #%00000000,DDRB

LOOP_START
       LDAA PORTB   ; Load PORTB into reg A, for check
       TSTA         ; Test reg A for minus and set N status bit accodingly (subtracts $00 from the content of accumulator A and sets the condition codes accordingly)
       LDAA PORTA   ; Already load PORTA into reg A (does not change N status bit)
       BMI INVERT   ; Branch if status bit N = 1
       STAA PORTA   ; Store reg A to PORTA
       JSR LOOP_START
INVERT
       COMA         ; Does the ones complement (aka inverts) content of reg A
       RTS