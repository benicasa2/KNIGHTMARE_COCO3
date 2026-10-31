        org $0b00
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
sample_loop
        lda $0be2
        lsla
        inca
        sta $0ccf
        ldx #$2700
        lda ,x
        cmpa #1
        bne sample_copy
        jsr $5660
sample_copy
        ldx #$2700
        ldy $0be0
        ldb #32
copy_loop
        lda ,x+
        sta ,y+
        decb
        bne copy_loop
        sty $0be0
        inc $0be2
        lda $0be2
        cmpa #180
        blo sample_loop

harness_done
        bra harness_done
