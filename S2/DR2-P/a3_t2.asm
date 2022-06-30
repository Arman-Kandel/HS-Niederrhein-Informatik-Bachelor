; START:
; Setup von Addressen, zum
; kontrollieren der LEDs.
;
; LOOP:
; Unterprogram a3_t1 ausführen.
; Z und C flag werden gelesen
; und bits auf LEDs 0, 1 übertragen.
;
ABSENTRY  MAIN
       ORG   $1000
MAIN   LDS   #4000 ; load stackpointer to last address

; Define ports
PORTA  EQU   $00
DDRA   EQU   $02
; Set PA7-PA0 to (1)OUTPUT (Ports zu LED Anzeige)
       MOVB #%11111111, DDRA

LOOP_START
       ; Execute a3_t1
       TFA          ; Transfer reg CRR to register A, to save CCR state: S X H I Z V C
       STAA PORTA   ; Show LEDs
       JSR LOOP_START