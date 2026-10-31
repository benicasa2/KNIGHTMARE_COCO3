        org $0b00
        lda #32
        sta $0c3b
        jsr $7cdb
        ldx #$0900
        ldu #$0941
        jsr [$0c91]
        stx $0be4
        stu $0be6

harness_done
        bra harness_done
