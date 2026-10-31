        org $0b00
        clr $0c40
        jsr $43a5
        ldd #64
        jsr $43ae

harness_done
        bra harness_done
