;-----------------------------------------------------------------------------------------
; Command buffer
;-----------------------------------------------------------------------------------------
    .rsset $0200
cmd_enabled .rs 1  ; Has work to be done
cmd_size    .rs 1  ; Current position of written data
cmd_data    .rs 256 - 2

CMD_SETPAL0 .func 0
CMD_SETPAL1 .func 1

;-----------------------------------------------------------------------------------------
; Wait for vblank to start
;-----------------------------------------------------------------------------------------
    .bank 0
WaitVBlank:
	BIT $2002
	BPL WaitVBlank
    rts

;-----------------------------------------------------------------------------------------
; Update an entire palette
; @$00 = Pointer to the palette
;-----------------------------------------------------------------------------------------
SetPal0:
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

SetPal1:
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
; When the game is done rendering, call this to submit your work to the PPU
;-----------------------------------------------------------------------------------------
SubmitCmd:
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
; NMI - Render what is in the command buffer!
;-----------------------------------------------------------------------------------------
    .bank 0
NMI:
	pha ; push
	txa
	pha
	tya
	pha

    ldx #0
NMI_CmdLoop:
    cpx cmd_size
    beq NMI_CmdDone
    lda cmd_data, x
    inx
    cmp #CMD_SETPAL0()
    beq NMI_CmdSetPal0
    cmp #CMD_SETPAL1()
    beq NMI_CmdSetPal1
    jmp NMI_CmdLoop

NMI_CmdSetPal0:
    LDA $2002
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
    LDA $2002
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
    rti ; Return from interupt
