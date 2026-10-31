        org $0b00
        orcc #$50
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
exit_trace_loop
        lda $0be2
        lsla
        inca
        sta $0ccf
        jsr $47db
        ldy $0be0
        lda $0db3
        sta ,y+
        lda $0c24
        ldb $0cb0
        std ,y++
        lda $0c25
        ldb $0cb1
        std ,y++
        ldd $0db5
        std ,y++
        ldd $0db7
        std ,y++
        lda $0db4
        sta ,y+
        lda $0c27
        sta ,y+
        sty $0be0
        inc $0be2
        lda $0be2
        cmpa #102
        lblo exit_trace_loop

harness_done
        bra harness_done
