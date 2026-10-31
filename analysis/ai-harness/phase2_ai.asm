        org $0b00
        orcc #$50
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
phase2_sample
        lda $0be2
        lsla
        inca
        sta $0ccf
        ldx #$2700
        lda ,x
        cmpa #1
        bne phase2_copy
        jsr $5660
phase2_copy
        ldx #$2700
        ldy $0be0
        ldb #32
phase2_copy_byte
        lda ,x+
        sta ,y+
        decb
        bne phase2_copy_byte
        sty $0be0
        inc $0be2
        lda $0be2
        cmpa #180
        blo phase2_sample

harness_done
        bra harness_done
