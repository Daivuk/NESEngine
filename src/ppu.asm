;-----------------------------------------------------------------------------------------
; Command buffer
;-----------------------------------------------------------------------------------------
    .rsset $0008
cmd_enabled .rs 1  ; Has work to be done
cmd_size    .rs 1  ; Current position of written data
    .rsset $0200
cmd_data    .rs 256

CMD_SETPAL0 .func 0
CMD_SETPAL1 .func 1
CMD_SETSCROLLING .func 2
CMD_END .func 3

    .bank 0

;-----------------------------------------------------------------------------------------
; Wait for vblank to start
;-----------------------------------------------------------------------------------------
WaitVBlank:
	BIT $2002
	BPL WaitVBlank
    rts

;-----------------------------------------------------------------------------------------
; Set horizontal and vertical scrolling
; @x, @y = position
;-----------------------------------------------------------------------------------------
ppu_SetScrolling:
    pha ; push
    tya
    pha

    ldy cmd_size
    lda #CMD_SETSCROLLING()
    sta cmd_data, y
    iny
    txa
    sta cmd_data, y
    iny
    pla
    sta cmd_data, y
    iny
    sty cmd_size
    
    pla
    rts

;-----------------------------------------------------------------------------------------
; Update an entire palette
; @$00 = Pointer to the palette
;-----------------------------------------------------------------------------------------
ppu_SetPal0:
    pha ; push
    txa
    pha
    tya
    pha

    ldx cmd_size
    lda #CMD_SETPAL0()
    sta cmd_data, x
    inx

    ldy #0
SetPal_loop0:
    lda [$00], y
    sta cmd_data, x
    inx
    iny
    cpy #16
    bne SetPal_loop0

    stx cmd_size

    pla ; pop
    tay
    pla
    tax
    pla
    rts

ppu_SetPal1:
    pha ; push
    txa
    pha
    tya
    pha

    ldx cmd_size
    lda #CMD_SETPAL1()
    sta cmd_data, x
    inx

    ldy #0
SetPal_loop1:
    lda [$00], y
    sta cmd_data, x
    inx
    iny
    cpy #16
    bne SetPal_loop1

    stx cmd_size

    pla ; pop
    tay
    pla
    tax
    pla
    rts

;-----------------------------------------------------------------------------------------
; Set end marker in the PPU
;-----------------------------------------------------------------------------------------
ppu_EndCmd:
	pha ; push
    txa
    pha

    ldx cmd_size
    lda #CMD_END()
    sta cmd_data, x

    pla ; pop
    tax
    pla
    rts

;-----------------------------------------------------------------------------------------
; When the game is done rendering, call this to submit your work to the PPU
;-----------------------------------------------------------------------------------------
ppu_SubmitCmd:
	pha ; push
    txa
    pha

    ldx cmd_size
    lda #CMD_END()
    sta cmd_data, x
    lda #1
    sta cmd_enabled
    lda #$ff
WaitCmdFeed:
    bit cmd_enabled
    bne WaitCmdFeed

    pla ; pop
    tax
    pla
    rts
    
;-----------------------------------------------------------------------------------------
; Wait for next vblank and then turn off the PPU
;-----------------------------------------------------------------------------------------
ppu_Off:
	pha ; Push stack
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
	pha ; Push stack
    jsr WaitVBlank
	lda #%10010000	; enable NMI, sprites from Pattern Table 0, bg pattern 1
	sta $2000
	lda #%00011110	; Enable rendering
	sta $2001
	pla ; Pop stack
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
	pha ; push
	txa
	pha
	tya
	pha

    lda #$ff
    bit cmd_enabled
    beq NMI_CmdDone

    ldx #0
NMI_CmdLoop:
    lda cmd_data, x
    inx
    asl A
    tay
    lda cmd_vtable, y
    sta $00
    iny
    lda cmd_vtable, y
    sta $01
    jmp [$0000]

NMI_CmdSetPal0:
    bit $2002
	LDA #$3F
	STA $2006
	LDA #$00
	STA $2006
	ldy #0
NMI_CmdSetPal_paletteLoop0:
	lda cmd_data, x
    inx
	sta $2007
	iny
	cpy #16
	bne NMI_CmdSetPal_paletteLoop0
    jmp NMI_CmdLoop

NMI_CmdSetPal1:
    bit $2002
	LDA #$3F
	STA $2006
	LDA #$10
	STA $2006
	ldy #0
NMI_CmdSetPal_paletteLoop1:
	lda cmd_data, x
    inx
	sta $2007
	iny
	cpy #16
	bne NMI_CmdSetPal_paletteLoop1
    jmp NMI_CmdLoop

NME_CmdSetScrolling:
    bit $2002
	lda cmd_data, x
    inx
	sta $2005
	lda cmd_data, x
    inx
	sta $2005
    lda #%10010000
	sta $2000
    jmp NMI_CmdLoop

NMI_CmdDone:
    ; Empty the command buffer
    lda #0
    sta cmd_size
    sta cmd_enabled
    
    pla ; pop
    tay
	pla
	tax
	pla
    rts
    
cmd_vtable:
    .dw NMI_CmdSetPal0, NMI_CmdSetPal1, NME_CmdSetScrolling, NMI_CmdDone
