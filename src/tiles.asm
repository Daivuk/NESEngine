    .bank 0

;-----------------------------------------------------------------------------------------
; Load a section at a specific screen in it
;-----------------------------------------------------------------------------------------
tiles_loadSection:
    jsr tiles_updateTileData
    
    PUSH_ALL

    lda game_zone_screen    ; Column * 16
    asl A
    asl A
    asl A
    asl A
    tax
    clc
    adc #16
    sta tmp7
    
tiles_loadSection_loopColumns:
    txa
    jsr getColumnAddr
    jsr drawColumn
    inx
    txa
    cmp tmp7
    bne tiles_loadSection_loopColumns
    
    POP_ALL
    rts

AddToAddr:
    clc
    adc $00
    sta $00
    lda $01
    adc #0
    sta $01
    rts

;-----------------------------------------------------------------------------------------
; Update zone addr
;-----------------------------------------------------------------------------------------
tiles_updateTileData:
    PUSH_ALL
    
    lda game_zone
    asl A                   ; Each zone offset is 2 bytes
    clc
    adc #5
    tay
    LOAD_ADDR worldData, 0  ; Setup the offset to the zone's first column
    lda [$00], y            ; Store the offset at $02
    sta $02
    iny
    lda [$00], y
    sta $03
    ADDW $00, $00, $02      ; Add base addr with offset, then +4 to skip the header
    lda #4
    jsr AddToAddr
    lda $00
    sta game_zone_pData
    lda $01
    sta game_zone_pData + 1
        
    LOAD_ADDR tiles, game_zone_pTiles      ; Tileset addr for this zone
    
    lda game_zone_pTiles
    sta tmp4
    lda game_zone_pTiles + 1
    sta tmp4 + 1
    lda #240
    ADD_TO_ADDR tmp4

    lda tmp4
    sta tmp5
    lda tmp4 + 1
    sta tmp5 + 1
    lda #240
    ADD_TO_ADDR tmp5

    lda tmp5
    sta tmp6
    lda tmp5 + 1
    sta tmp6 + 1
    lda #240
    ADD_TO_ADDR tmp6

    POP_ALL
    rts

;-----------------------------------------------------------------------------------------
; Get the address of a column
; @a = column ID
; tmp1 will have tile data address
; tmp2 will have vram address
;-----------------------------------------------------------------------------------------
getColumnAddr:
    pha
    clc                     ; a /= 16
    and #$F0
    ror A
    ror A
    ror A
    ror A
    adc game_zone_pData + 1     ; Start by saving the pointer to ROM data
    sta tmp1 + 1
    pla
    
    pha
    asl A
    asl A
    asl A
    asl A
    adc game_zone_pData
    sta tmp1
    lda tmp1 + 1
    adc #0
    sta tmp1 + 1
    pla
    
    and #$0F
    
    pha                     ; Nametable
    asl A                   ; Every 2 tiles offset
    clc                     ; We skip first 4 rows, that's for the status bar
    adc #128
    sta tmp2
    lda #0
    adc #$20
    sta tmp2 + 1
    pla
    
    pha
    and #$01
    bne getColumnAddr_bottomAttributes
    
    pla
    clc                     ; Attributes
    ror A
    clc
    adc #$C8
    sta tmp3
    lda #$23
    sta tmp3 + 1
    rts
    
getColumnAddr_bottomAttributes:
    pla
    clc                     ; Attributes
    ror A
    clc
    adc #$E0
    sta tmp3
    lda #$23
    sta tmp3 + 1
    rts

;-----------------------------------------------------------------------------------------
; Draw a column
; $00 = column address
;-----------------------------------------------------------------------------------------
drawColumn:
    PUSH_ALL
   
    bit $2002               ; read PPU status to reset the high/low latch
    ldy #0
    ldx #0
vLoop:
    lda tmp2 + 1            ; Write GPU address then increment 32 for next row
    sta $2006
    lda tmp2
    sta $2006
    lda #32
    ADD_TO_ADDR tmp2

    txa                     ; Write our 2 tiles
    tay
    lda [tmp1], y
    tay
    lda [game_zone_pTiles], y
    sta $2007
    lda [tmp4], y
    sta $2007
    
    lda tmp2 + 1            ; Write GPU address then increment 32 for next row
    sta $2006 
    lda tmp2
    sta $2006
    lda #32
    ADD_TO_ADDR tmp2

    lda [tmp5], y           ; Write the 2 next tiles
    sta $2007
    lda [tmp6], y
    sta $2007

    inx
    cpx #13
    bne vLoop               ; LOOP
    
    txa
    tay
    
    lda tmp3 + 1            ; Write GPU address then increment 32 for next row (Attributes row)
    sta $2006
    lda tmp3
    sta $2006
    
    lda [tmp1], y
    sta $2007
    
    lda #8
    ADD_TO_ADDR tmp3
    lda tmp3 + 1
    sta $2006
    lda tmp3
    sta $2006
    
    iny
    lda [tmp1], y
    sta $2007
    
    lda #8
    ADD_TO_ADDR tmp3
    lda tmp3 + 1
    sta $2006
    lda tmp3
    sta $2006
    
    iny
    lda [tmp1], y
    sta $2007
    lda #8

    POP_ALL
    rts
