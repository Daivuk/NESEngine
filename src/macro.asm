    .macro LOAD_ADDR
    lda #LOW(\1)
    sta $00
    lda #HIGH(\1)
    sta $01
    .endm
