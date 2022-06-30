; PROGRAMM: BITPOS
; START:
; Setze zahl/index (index von gewünschtem bit) in reg B.
; Wenn zahl < 0 oder > 7 setze Z flag und breche ab.
; Rechne 8 (reg A) - index (reg B), und setzt ergebnis in reg B.
; Setze bits in reg A.
;
; LOOP:
; Links shift reg A.
; Decrementiere reg B bis == 0, dann
; raus aus dem loop.
ABSENTRY  MAIN
       ORG   $1000
MAIN   LDS   #4000 ; load stackpointer to last address
       LDAB #3 ; Load bit index into B
       LDAA #7
       CBA ; Compare index (reg B) with 7 (reg A)
       BHI ABORT ; Branch if higher
       LDAA #0
       CBA ; Compare index (reg B) with 0 (reg A)
       BLS ABORT ; Branch if lower
LOOP_START
       TBNE B, DO_SHIFT_AND_LOOP ; Test and Branch (reg B) if Not Equal to Zero
       RTS
DO_SHIFT_AND_LOOP
       LSLA         ; Logic shift left reg A. Bit 7 goes over to C flag
       DECB ; Decrements reg B
       BRA LOOP_START
ABORT
       LDAA #0
       TSTA ; Tests A, and sets Z = 1, because its 0+0=0
       JSR ABORT