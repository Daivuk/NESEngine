    .rsset $0008
game_state: .rs 1
game_UNUSED: .rs 1
game_scroll: .rs 2

GAME_STATE_LOADING_SECTION .func 0
GAME_STATE_PLAY .func 0

    .bank 0

AddToAddr:
    clc
    adc $00
    sta $00
    lda $01
    adc #0
    sta $01
    rts

    .macro ADDW
    lda \3
    clc
    adc \2
    sta \1
    lda \3 + 1
    adc \2 + 1
    sta \1 + 1
    .endm

getColumnAddr:
    asl A           ; Each zone offset is 2 bytes
    clc
    adc #5
    tay

    LOAD_ADDR0 worldData ; Setup the offset to the zone's first column

    lda [$00], y    ; Store the offset at $02
    sta $02
    iny
    lda [$00], y
    sta $03

    ADDW $00, $00, $02  ; Add base addr with offset, then +4 to skip the header
    lda #4
    jsr AddToAddr

    txa
    asl A
    asl A
    asl A
    asl A
    jsr AddToAddr

    rts

drawColumn:
	bit $2002             ; read PPU status to reset the high/low latch
    ldx #0
    ldy #0
vLoop:
	lda #$20
	sta $2006             ; write the high byte of $2000 address
    txa
    asl A
    asl A
    asl A
    asl A
    asl A
	sta $2006             ; write the low byte of $2000 address

    tya
    pha
    lda [$00], y
    asl A
    asl A
    tay
    lda [$06], y
    sta $2007
    iny
    lda [$06], y
    sta $2007
    pla
    tay

    inx
	lda #$20
	sta $2006             ; write the high byte of $2000 address
    txa
    asl A
    asl A
    asl A
    asl A
    asl A
	sta $2006             ; write the low byte of $2000 address

    tya
    pha
    lda [$00], y
    asl A
    asl A
    tay
    iny
    iny
    lda [$06], y
    sta $2007
    iny
    lda [$06], y
    sta $2007
    pla
    tay
    iny

    inx
    cpx #8
    bne vLoop;

    rts

;-----------------------------------------------------------------------------------------
; Initialize method for the game
;-----------------------------------------------------------------------------------------
OnInit:
    pha

    ; Write tiles
    jsr ppu_Off

    ;LOAD_ADDR6 tiles
    ;lda #0 ; Sector 0
    ;ldx #0 ; Column 0
    ;jsr getColumnAddr
    ;jsr drawColumn

    jsr ppu_On

    LOAD_ADDR0 palWorld1            ; Default palette
    jsr ppu_SetPal0
    LOAD_ADDR0 palWorld1_sprites
    jsr ppu_SetPal1

    lda #SCROLL_DIR_HORIZONTAL()    ; Scrolling direction
    jsr ppu_SetScrollDir
    lda #BG_PATTERN1()              ; Our background tiles are on the second pattern
    jsr ppu_SetBGPattern

    ; Scrolling 0,0
    ;ldx #0
    ;ldy #0
    ;jsr ppu_SetScrolling

    pla
    rts

;-----------------------------------------------------------------------------------------
; Game main loop. This is called every frame by the engine
;-----------------------------------------------------------------------------------------
    .bank 0
OnFrame:
    lda game_scroll         ; Scroll X 1 per frame
    clc
    adc #1
    sta game_scroll
    sta $00
    lda game_scroll + 1
    adc #0
    sta game_scroll + 1
    sta $01
    jsr ppu_SetScrolling
    rts

;-----------------------------------------------------------------------------------------
; Include game data
;-----------------------------------------------------------------------------------------
    .include "src/data.asm"
