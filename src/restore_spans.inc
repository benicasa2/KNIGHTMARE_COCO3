; Unrolled background restore; selected once per sprite, widths 0..32.
restore_span_select
        lda <span_n
        lsla
        ldu #restore_span_table
        ldd a,u
        std <blit_fn
        rts
restore_span_table
        fdb restore_span_0,restore_span_1,restore_span_2,restore_span_3,restore_span_4,restore_span_5,restore_span_6,restore_span_7,restore_span_8,restore_span_9,restore_span_10,restore_span_11,restore_span_12,restore_span_13,restore_span_14,restore_span_15,restore_span_16,restore_span_17,restore_span_18,restore_span_19,restore_span_20,restore_span_21,restore_span_22,restore_span_23,restore_span_24,restore_span_25,restore_span_26,restore_span_27,restore_span_28,restore_span_29,restore_span_30,restore_span_31,restore_span_32
restore_span_31
        lda ,x+
        sta ,u+
        bra restore_span_30
restore_span_29
        lda ,x+
        sta ,u+
        bra restore_span_28
restore_span_27
        lda ,x+
        sta ,u+
        bra restore_span_26
restore_span_25
        lda ,x+
        sta ,u+
        bra restore_span_24
restore_span_23
        lda ,x+
        sta ,u+
        bra restore_span_22
restore_span_21
        lda ,x+
        sta ,u+
        bra restore_span_20
restore_span_19
        lda ,x+
        sta ,u+
        bra restore_span_18
restore_span_17
        lda ,x+
        sta ,u+
        bra restore_span_16
restore_span_15
        lda ,x+
        sta ,u+
        bra restore_span_14
restore_span_13
        lda ,x+
        sta ,u+
        bra restore_span_12
restore_span_11
        lda ,x+
        sta ,u+
        bra restore_span_10
restore_span_9
        lda ,x+
        sta ,u+
        bra restore_span_8
restore_span_7
        lda ,x+
        sta ,u+
        bra restore_span_6
restore_span_5
        lda ,x+
        sta ,u+
        bra restore_span_4
restore_span_3
        lda ,x+
        sta ,u+
        bra restore_span_2
restore_span_1
        lda ,x+
        sta ,u+
        bra restore_span_0
restore_span_32
        ldd ,x++
        std ,u++
restore_span_30
        ldd ,x++
        std ,u++
restore_span_28
        ldd ,x++
        std ,u++
restore_span_26
        ldd ,x++
        std ,u++
restore_span_24
        ldd ,x++
        std ,u++
restore_span_22
        ldd ,x++
        std ,u++
restore_span_20
        ldd ,x++
        std ,u++
restore_span_18
        ldd ,x++
        std ,u++
restore_span_16
        ldd ,x++
        std ,u++
restore_span_14
        ldd ,x++
        std ,u++
restore_span_12
        ldd ,x++
        std ,u++
restore_span_10
        ldd ,x++
        std ,u++
restore_span_8
        ldd ,x++
        std ,u++
restore_span_6
        ldd ,x++
        std ,u++
restore_span_4
        ldd ,x++
        std ,u++
restore_span_2
        ldd ,x++
        std ,u++
restore_span_0
        rts
