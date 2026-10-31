        org $0b00
        orcc #$50
        lda #12
        sta $ffa4
        ldy #$8000
        sty $0be0
        clr $0be2
map_band_loop
        lda $0be2
        jsr $43e0
        ldx #$0400
        ldy $0be0
        ldb #128
map_copy_band
        lda ,x+
        sta ,y+
        decb
        bne map_copy_band
        sty $0be0
        inc $0be2
        lda $0be2
        cmpa #61
        blo map_band_loop

harness_done
        bra harness_done
