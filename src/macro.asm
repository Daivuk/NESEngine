    .macro LOAD_ADDR
    lda #LOW(\1)
    sta \2
    lda #HIGH(\1)
    sta \2 + 1
    .endm
	
    .macro ADDW
    lda \3
    clc
    adc \2
    sta \1
    lda \3 + 1
    adc \2 + 1
    sta \1 + 1
    .endm
    
    .macro ADD_TO_ADDR
    clc
    adc \1
    sta \1
    lda \1 + 1
    adc #0
    sta \1 + 1
    .endm

    .macro PUSH_ALL
    pha
    txa
    pha
    tya
    pha
    .endm
    
    .macro POP_ALL
    pla
    tay
    pla
    tax
    pla
    .endm

    .macro PUSH_Y
    pha
    tya
    pha
    .endm
    
    .macro POP_Y
    pla
    tay
    pla
    .endm

    .macro PUSH_X
    pha
    txa
    pha
    .endm
    
    .macro POP_X
    pla
    tax
    pla
    .endm
    