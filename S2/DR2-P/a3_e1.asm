       ORG   $1000    ; Begin Program UPTEST
RAMEND EQU   $3FFF    ; =
UPTEST LDS   #RAMEND+1; load stackpointer
       LDX   #$0000   ; load register x
       LDD   #$FFFF   ; load double accumulator
       JSR   SUB1     ; jump to subroutine
       STD   DREG     ; store double accumulator
       STX   XREG     ; store register x
FINI   JMP   FINI     ; End Program UPTEST
DREG   DS.W  1        ; Local Data Area
XREG   DS.W  1
       ORG   $1100    ; Begin Subroutine SUB 1
SUB1   CLRA           ; clear a
       PSHB           ; decrement stackpointer; store b at stackpointer address; 
       JSR   SUB2     ; jump to subroutine
       PULB           ; load SP value into B; increment SP;
       RTS            ; End Subroutine SUB 1
       ORG   $1200    ; Begin Subroutine SUB 2
SUB2   LDAB  2,SP     ; load accoumulator b
       JSR   SUB3     ; jump to subroutine
       STAB  2,SP     ; store accumulator b
       RTS            ; End Subroutine SUB 2
       ORG   $1300    ; Begin Subroutine SUB 3
SUB3   CLRB           ; clear accumulator b
       LDX   #$FFFF   ; load register x
       RTS            ; End SUB3; go to SUB2 STAB  

       ;DREG und XREG haben beide den Wert 0 am ende des programs.
     