        org $0b00
        jsr $5487
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be4
        clr $0be5
wave_sample_loop
        lda $0be5
        lsla
        inca
        sta $0ccf
        ldd $0be4
        lsra
        rorb
        lsra
        rorb
        std $0c0c
        jsr $5498
        ldx #$2700
        clr $0be6
wave_log_slot
        lda ,x
        cmpa #1
        bne wave_log_next
        lda 12,x
        cmpa #1
        bne wave_log_next
        ldy $0be0
        ldd $0be4
        std ,y++
        lda $0be6
        sta ,y+
        lda 26,x
        sta ,y+
        lda 15,x
        sta ,y+
        ldd 2,x
        std ,y++
        ldd 4,x
        std ,y++
        sty $0be0
wave_log_next
        leax 32,x
        inc $0be6
        lda $0be6
        cmpa #7
        blo wave_log_slot
        jsr $5640
        ldd $0be4
        addd #1
        std $0be4
        cmpd #6000
        lblo wave_sample_loop

harness_done
        bra harness_done
