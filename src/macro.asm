    .macro LOAD_ADDR0
    lda #LOW(\1)
    sta $00
    lda #HIGH(\1)
    sta $01
    .endm

    .macro LOAD_ADDR2
    lda #LOW(\1)
    sta $02
    lda #HIGH(\1)
    sta $03
    .endm

    .macro LOAD_ADDR4
    lda #LOW(\1)
    sta $04
    lda #HIGH(\1)
    sta $05
    .endm

    .macro LOAD_ADDR6
    lda #LOW(\1)
    sta $06
    lda #HIGH(\1)
    sta $07
    .endm
