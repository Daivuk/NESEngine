;-----------------------------------------------------------------------------------------
; Command buffer
;-----------------------------------------------------------------------------------------
    .rsset $0080
cmd_enabled     .rs 1   ; 80 Has work to be done
cmd_dirtyBits   .rs 1   ; 81 Dirty bitfield for PPU
; 7654 3210
; |||| ||||
; |||| |||+- 0 Scroll dirty
; |||| ||+-- 1 Palette0 dirty
; |||| |+--- 2 Palette1 dirty
; |||| +---- 3
; |||+------ 4
; ||+------- 5
; |+-------- 6
; +--------- 7
cmd_scrollX     .rs 1   ; 82 x scroll position
cmd_scrollY     .rs 1   ; 83 y scroll position
cmd_bgFlags     .rs 1   ; 84 Background flags
; 7654 3210
;    |  |||
;    |  ||+- 0 Add 255 to X scroll
;    |  |+-- 1 Add 240 to Y scroll
;    |  +--- 2 Direction.
;    |         (0 = horizontal, 1 = vertical)
;    +------ 4 Background pattern table
cmd_pal0        .rs 16  ; 85 bg palette
cmd_pal1        .rs 16  ; 9B sprite palette
                        ; B1

;-----------------------------------------------------------------------------------------
; Constants
;-----------------------------------------------------------------------------------------
DIRTY_SCROLL            .func %1
DIRTY_PAL0              .func %10
DIRTY_PAL1              .func %100
SCROLL_DIR_HORIZONTAL   .func %0
SCROLL_DIR_VERTICAL     .func %100
BG_PATTERN0             .func %0
BG_PATTERN1             .func %10000

    .rsset $0200
cmd_data    .rs 256

    .bank 0
;-----------------------------------------------------------------------------------------
; Wait for vblank to start
;-----------------------------------------------------------------------------------------
WaitVBlank:
	BIT $2002
	BPL WaitVBlank
    rts

;-----------------------------------------------------------------------------------------
; Set scrolling direction
; a = SCROLL_DIR_HORIZONTAL or SCROLL_DIR_VERTICAL
;-----------------------------------------------------------------------------------------
ppu_SetScrollDir:
    pha
    lda #%11111011
    and cmd_bgFlags
    sta cmd_bgFlags
    pla
    ora cmd_bgFlags
    sta cmd_bgFlags
    rts

;-----------------------------------------------------------------------------------------
; Set background Pattern Table
; a = BG_PATTERN0 or BG_PATTERN1
;-----------------------------------------------------------------------------------------
ppu_SetBGPattern:
    pha
    lda #%11101111
    and cmd_bgFlags
    sta cmd_bgFlags
    pla
    ora cmd_bgFlags
    sta cmd_bgFlags
    rts

;-----------------------------------------------------------------------------------------
; Set scrolling position
; @$00 = 16 bits scroll value
;-----------------------------------------------------------------------------------------
ppu_SetScrolling:
    pha
    lda #%00000100
    bit cmd_bgFlags
    bne ppu_SetScrolling_doV
    lda #0
    sta cmd_scrollY
    lda $00
    sta cmd_scrollX
    lda $01
    and #%00000001
    sta $00
    lda cmd_bgFlags
    and #%11111100
    ora $00
    sta cmd_bgFlags
    jmp ppu_SetScrolling_done
ppu_SetScrolling_doV:
    lda #0
    sta cmd_scrollX

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  16 bit mod 240
;  By Omegamatrix
;  39-42 cycles, 32 bytes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    lda    $01                   ;3  @3
    and    #$F0                  ;2  @5
    sta    cmd_scrollY           ;3  @8
    eor    $01                   ;3  @11
    asl    A                     ;2  @13
    asl    A                     ;2  @15
    asl    A                     ;2  @17
    asl    A                     ;2  @19
    adc    cmd_scrollY           ;3  @22
    bcc    .doneHigh             ;2³ @24/25
    adc    #(1 << 4) - 1         ;2  @26      -1 because carry is set
.doneHigh:
    adc    $00                   ;3  @29
    bcc    .try240               ;2³ @31/32
    adc    #16-1                 ;2  @33      -1 because carry is set
.try240:
    cmp    #240                  ;2  @35
    bcc    .storeRemainder       ;2³ @37/38
    sbc    #240                  ;2  @39
.storeRemainder:
    sta    cmd_scrollY           ;3  @42
    
    ppu_SetScrolling_done:
    lda #DIRTY_SCROLL()
    ora cmd_dirtyBits
    sta cmd_dirtyBits
    pla
    rts

;-----------------------------------------------------------------------------------------
; Update palette 0
; @$00 = Pointer to the palette
;-----------------------------------------------------------------------------------------
ppu_SetPal0:
    pha ; push
    tya
    pha

    ldy #0
SetPal0_loop:
    lda [$00], y
    sta cmd_pal0, y
    iny
    cpy #16
    bne SetPal0_loop

    lda #DIRTY_PAL0()
    ora cmd_dirtyBits
    sta cmd_dirtyBits

    pla ; pop
    tay
    pla
    rts

;-----------------------------------------------------------------------------------------
; Update palette 1
; @$00 = Pointer to the palette
;-----------------------------------------------------------------------------------------
ppu_SetPal1:
    pha ; push
    tya
    pha

    ldy #0
SetPal1_loop:
    lda [$00], y
    sta cmd_pal1, y
    iny
    cpy #16
    bne SetPal1_loop

    lda #DIRTY_PAL1()
    ora cmd_dirtyBits
    sta cmd_dirtyBits

    pla ; pop
    tay
    pla
    rts

;-----------------------------------------------------------------------------------------
; When the game is done rendering, call this to submit your work to the PPU
;-----------------------------------------------------------------------------------------
ppu_Submit:
	pha ; push

    lda #1
    sta cmd_enabled
    lda #$ff
WaitCmdFeed:
    bit cmd_enabled
    bne WaitCmdFeed

    pla ; pop
    rts
    
;-----------------------------------------------------------------------------------------
; Wait for next vblank and then turn off the PPU
;-----------------------------------------------------------------------------------------
ppu_Off:
	pha ; Push stack
    lda #%011111111 ; disable NMI
    and cmd_bgFlags
    sta cmd_bgFlags
    jsr WaitVBlank
	lda #$00
	sta $2000
	sta $2001
	pla ; Pop stack
    rts
    
;-----------------------------------------------------------------------------------------
; Wait for next vblank and then turn on the PPU
;-----------------------------------------------------------------------------------------
ppu_On:
    jsr WaitVBlank
    pha
    lda #%10000000 ; enable NMI
    ora cmd_bgFlags
    sta cmd_bgFlags
    bit $2002
	sta $2000
	lda #%00011110
	sta $2001
    pla
    rts
    
;-----------------------------------------------------------------------------------------
; NMI - Will execute the current command buffer
;-----------------------------------------------------------------------------------------
NMI:
    jsr ppu_ExecuteCmd
    rti ; Return from interupt
    
;-----------------------------------------------------------------------------------------
; Render what is in the command buffer!
;-----------------------------------------------------------------------------------------
ppu_ExecuteCmd:
	pha                     ; push
	txa
	pha
	tya
	pha
    lda #$ff                ; Skip is not enabled
    bit cmd_enabled
    beq NMI_CmdDone

    lda #DIRTY_PAL0()       ; Update pal0
    bit cmd_dirtyBits
    beq NMI_skipPal0
    bit $2002
    lda #$3F
    sta $2006
    lda #$00
    sta $2006
    ldy #0
NMI_pal0Loop:
	lda cmd_pal0, y
	sta $2007
	iny
	cpy #16
	bne NMI_pal0Loop

NMI_skipPal0:               ; Update pal1
    lda #DIRTY_PAL1()           
    bit cmd_dirtyBits
    beq NMI_skipPal1
    bit $2002
    lda #$3F
    sta $2006
    lda #$10
    sta $2006
    ldy #0
NMI_pal1Loop:
	lda cmd_pal1, y
	sta $2007
	iny
	cpy #16
	bne NMI_pal1Loop

NMI_skipPal1:               ; Update scrolling - Always dirty
    ;lda #DIRTY_SCROLL()           
    ;bit cmd_dirtyBits
    ;beq NMI_skipScroll
    bit $2002
    lda cmd_scrollX
    sta $2005
    lda cmd_scrollY
    sta $2005

NMI_skipScroll:             ; Empty the command buffer
NMI_CmdDone:
    bit $2002               
    lda cmd_bgFlags         ; Always set back the default bg flags
    sta $2000
    lda #0
    sta cmd_dirtyBits
    sta cmd_enabled
    pla                     ; pop
    tay
	pla
	tax
	pla
    rts
   
 ; SAVE FOR LATER, VTABLE STUFF
;    ldx #0
;NMI_CmdLoop:
;    lda cmd_data, x
;    inx
;    asl A
;    tay
;    lda cmd_vtable, y
;    sta $00
;    iny
;    lda cmd_vtable, y
;    sta $01
;    jmp [$0000]
;cmd_vtable:
;    .dw NMI_CmdSetPal0, NMI_CmdSetPal1, NME_CmdSetScrolling, NMI_CmdDone
