;-----------------------------------------------------------------------------------------
; iNES header
;-----------------------------------------------------------------------------------------
    .inesprg 2   ; 2x 16KB PRG code
    .ineschr 1   ; 1x 8KB CHR data
    .inesmir 1   ; background mirroring
    .inesmap 0   ; mapper 0 = NROM, no bank swapping
    	
;-----------------------------------------------------------------------------------------
; Interrupts
;-----------------------------------------------------------------------------------------
	.bank 3
	.org $FFFA     ;first of the three vectors starts here
	.dw NMI        ;when an NMI happens (once per frame if enabled) the processor will jump to the label NMI:
	.dw RESET      ;when the processor first turns on or is reset, it will jump to the label RESET:
	.dw 0          ;external interrupt IRQ is not used in this engine

;-----------------------------------------------------------------------------------------
; CHR banks
;-----------------------------------------------------------------------------------------
	.bank 4
	.org $0000
	.incbin "assets/pattern.chr"

;-----------------------------------------------------------------------------------------
; Game code bank
;-----------------------------------------------------------------------------------------
	.bank 0
	.org $8000 

;-----------------------------------------------------------------------------------------
; Turn ON, Reset. Start of the program
;-----------------------------------------------------------------------------------------
	.bank 0
RESET:
	SEI          ; disable IRQs
	CLD          ; disable decimal mode
	LDX #$40
	STX $4017    ; disable APU frame IRQ
	LDX #$FF
	TXS          ; Set up stack
	INX          ; now X = 0
	STX $2000    ; disable NMI
	STX $2001    ; disable rendering
	STX $4010    ; disable DMC IRQs
	jsr WaitVBlank
clrmem:
	LDA #$00
	STA $0000, x
	STA $0100, x
	STA $0200, x
	STA $0400, x
	STA $0500, x
	STA $0600, x
	STA $0700, x
	LDA #$FE
	STA $0300, x
	INX
	BNE clrmem
	jsr WaitVBlank

	ldx #$10			; Wait couple frames
	jsr Wait			;---
	jmp Main

;-----------------------------------------------------------------------------------------
; Includes
;-----------------------------------------------------------------------------------------
    .include "src/macro.asm"
    .include "src/ppu.asm"
    .include "src/timing.asm"

;-----------------------------------------------------------------------------------------
; Main loop
;-----------------------------------------------------------------------------------------
	.bank 0
Main:
    lda #%10010000	; enable NMI, sprites from Pattern Table 0, bg pattern 1
	sta $2000
	lda #%00011110	; Enable rendering
	sta $2001
    jsr Game
    jsr SubmitCmd
MainLoop:
    ;jsr Game
    jmp MainLoop

;-----------------------------------------------------------------------------------------
; Lastly, includes our game code
;-----------------------------------------------------------------------------------------
    .include "src/game.asm"
