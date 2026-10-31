        org $0b00
        orcc #$50
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
boss2_trace_loop
        lda $0be5
        lsla
        inca
        sta $0ccf
        jsr $643e
        ldy $0be0
        lda $0c59
        ldb $0dbc
        std ,y++
        lda $0c5a
        ldb $0dbd
        std ,y++
        ldd $0dbe
        std ,y++
        ldd $0dc0
        std ,y++
        lda $0dc2
        sta ,y+
        ldd $0ce0
        std ,y++
        ldx #$2840
        ldb #160
boss2_copy_drops
        lda ,x+
        sta ,y+
        decb
        bne boss2_copy_drops
        sty $0be0
        inc $0be5
        inc $0be2
        lda $0be2
        cmpa #40
        lblo boss2_trace_loop

harness_done
        bra harness_done
