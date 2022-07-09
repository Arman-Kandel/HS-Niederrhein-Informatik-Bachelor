; -------------------------------------------------------------------
; Hauptprogramm: TESTMM
          ABSENTRY MAIN       ; mark this as application entry point

          ORG   $1000         ; Def. Basisadr. des Code im RAM 
MAIN      LDS   #$4000        ; Init stack
; Define ports
PORTA     EQU   $00
PORTB     EQU   $01
DDRA      EQU   $02
DDRB      EQU   $03
          MOVB #%00000000,DDRA   ; Set PA7...PA0 to (0)INPUT (Schalter)
          MOVB #%11111111,DDRB   ; Set PB7...PB0 (1)OUTPUT (LED)

          LDX   #$3000 ;Start-Adresse; Zum reservieren von speicher (8bit) kann EQU oder DS.B 1 benutzt werden
          LDY   #$300F ;End-Adresse
          
MAIN_LOOP 
          LDAA  PORTA  ; Use value from PORTA to replace data
          JSR   DEFMEM
          TFR   CCR,B
          STAB  PORTB ; Store status code into PORTB
          JMP   MAIN_LOOP

          
; -------------------------------------------------------------------
; Unterprogramm: DEFMEM(X,Y,A,Z_Flag)
          ORG     $1200
DEFMEM    PSHX            ; Register retten           
          PSHY            ; Endadresse auf den Stack kopieren
          CPX     0,SP    ; Flags=X-Y
                          ;   (Vergleich von vorzeichenlosen 16b-Zahlen)
          BHI     ERROR  ; IF X>Y --> Fehler        (ERRORA)
          BRA     DEFMEM_LOOP ; ELSE   --> Finde Maximum (FNDMAXA)

ERROR     ORCC    #$04    ; Z-Flag setzen (=1)
          BRA     DONE

DEFMEM_LOOP
          STAA    X
          INX             ;   Nächstes Element (Listenadresse + 1)
          CPX     0,SP    ;   Flags=X-Y
          BLS     DEFMEM_LOOP ; WHILE (X<=Y)
          ANDCC   #$FB    ; Z-Flat zurücksetzen (=0) 
          
DONE      INS             ; Konstante 16bit-Variable 
          INS             ;   vom Stack entfernen (Endadresse)
          PULX            ; Register wiederherstellen
          RTS             ; RETURN     
          
           
