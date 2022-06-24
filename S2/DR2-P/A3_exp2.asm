; (PORT A) PA0, PA1, PA2, PA3 = INPUT Schalter
; S0, S1, S2, S3 = Schalter
; D0, D1, D2, D3 = OUTPUT LEDs, zeige Status von Schalter
; (PORT A) PA4, PA5, PA6, PA7 = OUTPUT Anzeige 
; (PORT B) PB7 = INPUT, wenn == 1 invertiere PA0 bis PA3
; S7 = Schalter von PB7
; Endlosschleife wenn stackpointer und port richtungen initialisiert sind.

       ORG   $1000    ; Start
RAMEND EQU   $3FFF    ; =
       LDS   #RAMEND+1; load stackpointer to last address
; Reserve memory space (1 byte each) for INPUT Schalter
S0     DS.B  1
S1     DS.B  1
S2     DS.B  1
S3     DS.B  1
; Reserve memory space for INPUT ports A
PA0    DS.B  1
PA1    DS.B  1
PA2    DS.B  1
PA3    DS.B  1
; Reserve memory space for OUTPUT LEDs
D0     DS.B  1
D1     DS.B  1
D2     DS.B  1
D3     DS.B  1
; Reserve memory space for OUTPUT ports A, Anzeige
PA4    DS.B  1
PA5    DS.B  1
PA6    DS.B  1
PA7    DS.B  1
; Reserve memory space for special INPUT port B: PB7
PB7    DS.B  1

MAIN
       LDAA PB7      ; Wenn == 1 invertiere PA0 bis PA3
       TBNE A,INVERT ; Jump to branch if A != 0
       JSR SET_LEDS
       JSR SET_DISPLAY
       JSR MAIN

SET_LEDS 
       LDAA PA0
       STAA D0

       LDAA PA1
       STAA D1

       LDAA PA2
       STAA D2

       LDAA PA3
       STAA D3
       RTS

SET_DISPLAY
       LDAA PA0
       STAA PA4

       LDAA PA1
       STAA PA5

       LDAA PA2
       STAA PA6

       LDAA PA3
       STAA PA7
       RTS

INVERT
       LDAA PA0
       TBNE A,SET_0 ; Jump to branch if A != 0
       TBEQ A,SET_1 ; Jump to branch if A == 0
       STAA PA0

       LDAA PA1
       TBNE A,SET_0 ; Jump to branch if A != 0
       TBEQ A,SET_1 ; Jump to branch if A == 0
       STAA PA1

       LDAA PA2
       TBNE A,SET_0 ; Jump to branch if A != 0
       TBEQ A,SET_1 ; Jump to branch if A == 0
       STAA PA2

       LDAA PA3
       TBNE A,SET_0 ; Jump to branch if A != 0
       TBEQ A,SET_1 ; Jump to branch if A == 0
       STAA PA3

       RTS

SET_0
       LDAA #0
       RTS
SET_1
       LDAA #1
       RTS
