        org $0b00
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
boss_sample_loop
        lda $0be5
        lsla
        inca
        sta $0ccf
        lda $0be5
        anda #127
        adda #64
        sta $0c24
        jsr $6291
        inc $0ccf
        jsr $5640
        dec $0ccf
        ldy $0be0
        ldx #$0c57
        ldb #8
boss_copy
        lda ,x+
        sta ,y+
        decb
        bne boss_copy
        lda $0cdf
        sta ,y+
        lda $0ce0
        sta ,y+
        lda $0ce1
        sta ,y+
        lda $0ce2
        sta ,y+
        lda $0ce3
        sta ,y+
        lda $0ce4
        sta ,y+
        lda $0ce5
        sta ,y+
        lda $0ce6
        sta ,y+
        ldx #$2700
        ldb #96
sat_copy
        lda ,x+
        sta ,y+
        decb
        bne sat_copy
        sty $0be0
        ldd $0be4
        addd #1
        std $0be4
        inc $0be2
        lda $0be2
        cmpa #64
        lblo boss_sample_loop

harness_done
        bra harness_done
