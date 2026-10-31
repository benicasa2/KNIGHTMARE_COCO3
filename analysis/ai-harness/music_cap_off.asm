        org $0b00
        jsr $31e2
        lda #1
        sta $0a00
        ldy #$0a00
        jsr $5585

harness_done
        bra harness_done
