        org $0b00
        orcc #$50
        lda #1
        sta $0c40
        jsr $550d

harness_done
        bra harness_done
