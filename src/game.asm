;-----------------------------------------------------------------------------------------
; Game main loop. This is called every frame by the engine
;-----------------------------------------------------------------------------------------
    .bank 0
Game:
    ;LOAD_ADDR palWorld1

    lda #LOW(palWorld1)
    sta $00
    lda #HIGH(palWorld1)
    sta $01

    jsr SetPal0
    jsr SetPal1
    rts

;-----------------------------------------------------------------------------------------
; Include game data
;-----------------------------------------------------------------------------------------
    .include "src/data.asm"