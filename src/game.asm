    .rsset $0010
game_state:             .rs 1   ; x10 Game state
game_UNUSED:            .rs 1   ; x11 
game_scroll:            .rs 2   ; x12 X or Y scroll position within a zone
game_zone:              .rs 1   ; x14 Zone ID currently in
game_zone_screen:       .rs 1   ; x15 Screen to span in the zone. Used only for loading
game_zone_dir:          .rs 1   ; x16 Zone direction
game_zone_size:         .rs 1   ; x17 Zone size, starting at x,y going in direction
game_zone_pos:          .rs 2   ; x18 Zone position in world map
game_zone_pData:        .rs 2   ; x1A Pointer to zone data
game_zone_pTiles0:      .rs 2   ; x1C Pointer to zone tiles data
game_zone_pTiles1:      .rs 2   ; x1E Pointer to zone tiles data
game_zone_pTiles2:      .rs 2   ; x20 Pointer to zone tiles data
game_zone_pTiles3:      .rs 2   ; x22 Pointer to zone tiles data
game_zone_palette:      .rs 16  ; x24 Currently used palette
game_pPalettes:         .rs 2   ; x34 All palettes pointer. 32 x 4 colours

GAME_STATE_LOADING_SECTION .func 0
GAME_STATE_PLAY .func 0

    .bank 0
    .include "src/tiles.asm"

;-----------------------------------------------------------------------------------------
; Initialize method for the game
;-----------------------------------------------------------------------------------------
OnInit:
    pha

    jsr ppu_Off             ; Begin loading

    lda #5                  ; We start at section 0
    sta game_zone
    lda #0                  ; In screen 0
    sta game_zone_screen
    jsr tiles_loadSection

    jsr ppu_On              ; Done loading

    LOAD_ADDR game_zone_palette, tmp1            ; Default palette
    jsr ppu_SetPal0
    LOAD_ADDR palWorld1_sprites, tmp1
    jsr ppu_SetPal1

    lda #BG_PATTERN1()              ; Our background tiles are on the second pattern
    jsr ppu_SetBGPattern

    pla
    rts

;-----------------------------------------------------------------------------------------
; Game main loop. This is called every frame by the engine
;-----------------------------------------------------------------------------------------
    .bank 0
OnFrame:
    ;lda game_scroll         ; Scroll X 1 per frame
    ;clc
    ;adc #1
    ;sta game_scroll
    ;sta $00
    ;lda game_scroll + 1
    ;adc #0
    ;sta game_scroll + 1
    ;sta $01
    ;jsr ppu_SetScrolling
    rts

;-----------------------------------------------------------------------------------------
; Include game data
;-----------------------------------------------------------------------------------------
    .include "src/data.asm"
