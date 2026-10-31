        org $0b00
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
music_verify_sample
        ldy $0be0
        ldd $0d8e
        std ,y++
        ldd $0d9e
        std ,y++
        lda $0d87
        sta ,y+
        lda $0d97
        sta ,y+
        lda $0d8b
        sta ,y+
        lda $0d9b
        sta ,y+
        ldd $0cf7
        std ,y++
        ldd $0cf9
        std ,y++
        lda $0cfd
        sta ,y+
        sty $0be0
        jsr $3229
        dec $0be2
        bne music_verify_sample

harness_done
        bra harness_done
