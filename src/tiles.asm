    .bank 0

;-----------------------------------------------------------------------------------------
; Load a section at a specific screen in it
;-----------------------------------------------------------------------------------------
tiles_loadSection:
    jsr tiles_updateTileData
    
    PUSH_ALL

    ; We will draw (zone * 16 - 8) to (zone * 16 + 23)
    ; In other words, a screen (16 columns) with extra
    ; 8 columns on each sides
    lda game_zone_screen    ; Column * 16
    asl A
    asl A
    asl A
    asl A
    tax
    
    clc                                     ; Add 24
    adc #24
    cmp game_zone_size                      ; Clamp to zone size - 1
    bmi tiles_loadSection_skipClampSize
    lda game_zone_size
tiles_loadSection_skipClampSize:
    sta tmp7
    
    txa                                     ; Subtract 8            
    clc
    adc #-8
    cmp #248                                ; Clamp to 0
    bcc tiles_loadSection_skipClampToZero
    lda #0
tiles_loadSection_skipClampToZero:
    tax
    
    ; Loop the columns
tiles_loadSection_loopColumns:
    txa
    jsr drawColumn
    inx
    txa
    cmp tmp7
    bne tiles_loadSection_loopColumns
    
    ; Set initial scroll position
    lda game_zone_screen
    sta tmp1 + 1
    lda #0
    sta tmp1
    jsr ppu_SetScrolling
    
    POP_ALL
    rts
    
;-----------------------------------------------------------------------------------------
; Copies a 4 color palette
;-----------------------------------------------------------------------------------------
copyPalette:
    lda [tmp1], y
    asl A
    asl A
    tay
    lda worldPalettes, y
    sta game_zone_palette, x
    iny
    inx
    lda worldPalettes, y
    sta game_zone_palette, x
    iny
    inx
    lda worldPalettes, y
    sta game_zone_palette, x
    iny
    inx
    lda worldPalettes, y
    sta game_zone_palette, x
    inx
    rts

;-----------------------------------------------------------------------------------------
; Update zone addr
;-----------------------------------------------------------------------------------------
tiles_updateTileData:
    PUSH_ALL
    
    lda game_zone
    asl A                       ; Each zone offset is 2 bytes
    tay
    LOAD_ADDR worldData, tmp1   ; Setup the offset to the zone's first column
    lda [tmp1], y               ; Store the offset at $02
    sta tmp2
    iny
    lda [tmp1], y
    sta tmp2 + 1
    ADDW tmp1, tmp1, tmp2       ; Add base addr with offset
    
    lda (tmp1)                  ; Load zone properties
    sta game_zone_dir
    ldy #1
    lda [tmp1], y
    asl A
    asl A
    asl A
    asl A
    sta game_zone_size
    
    ldy #4                      ; Copy zone palette
    ldx #0
    jsr copyPalette
    ldy #5 
    jsr copyPalette
    ldy #6
    jsr copyPalette
    ldy #7
    jsr copyPalette
    
    lda #8                      ; Skip header
    ADD_TO_ADDR tmp1
    
    lda tmp1                    ; Store our data pointer
    sta game_zone_pData
    lda tmp1 + 1
    sta game_zone_pData + 1
        
    LOAD_ADDR tiles, game_zone_pTiles   ; Tileset addr for this zone
    
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
; tmp2 will have vram tile address
; tmp3 will have vram attributes address
;-----------------------------------------------------------------------------------------
getColumnAddr:
    ; Tile data is at:
    ; Column * 16
    pha                     
    clc                     ; a /= 16
    and #$F0
    ror A
    ror A
    ror A
    ror A
    adc game_zone_pData + 1 ; Start by saving the pointer to ROM data
    sta tmp1 + 1
    pla
    
    pha
    asl A
    asl A
    asl A
    asl A
    clc
    adc game_zone_pData
    sta tmp1
    lda tmp1 + 1
    adc #0
    sta tmp1 + 1
    lda #$20
    sta tmp8
    lda #$23
    sta tmp8 + 1
    lda #%00010000
    sta tmp2
    pla
    
    bit tmp2
    beq getColumnAddr_skipOffPage
    pha
    lda #$24
    sta tmp8
    lda #$27
    sta tmp8 + 1
    pla
    
getColumnAddr_skipOffPage:
    and #$0F
    pha                     ; Nametable
    asl A                   ; Every 2 tiles offset
    ora #%10000000          ; +128 because We ignore the 4 first rows for status bar
    sta tmp2
    lda tmp8
    sta tmp2 + 1
    pla
  
    pha
    and #%00000001
    bne getColumnAddr_bottomAttributes
    
    pla
    clc                     ; Attributes
    ror A
    clc
    adc #$C8
    sta tmp3
    lda tmp8 + 1
    sta tmp3 + 1
    rts
    
getColumnAddr_bottomAttributes:
    pla
    clc                     ; Attributes
    ror A
    clc
    adc #$E0
    sta tmp3
    lda tmp8 + 1
    sta tmp3 + 1
    rts

;-----------------------------------------------------------------------------------------
; Draw a column
; $00 = column address
;-----------------------------------------------------------------------------------------
drawColumn:
    jsr getColumnAddr

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
    
    tay
    lda #$01
    bit tmp7
    bne drawColumn_skipBottomRow
    
    tya                 ; Bottom row is repeated previous row
    ror A
    ror A
    ror A
    ror A
    pha
    lda #8
    ADD_TO_ADDR tmp3
    lda tmp3 + 1
    sta $2006
    lda tmp3
    sta $2006
    pla
    sta $2007

drawColumn_skipBottomRow:
    POP_ALL
    rts
