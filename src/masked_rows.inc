; Unrolled masked rows, sharing fall-through code; width-indexed entry.
masked_row_table
        fdb masked_row_0,masked_row_1,masked_row_2,masked_row_3,masked_row_4,masked_row_5,masked_row_6,masked_row_7,masked_row_8,masked_row_9
masked_row_9
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
masked_row_8
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
masked_row_7
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
masked_row_6
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
masked_row_5
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
masked_row_4
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
masked_row_3
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
masked_row_2
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
masked_row_1
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
masked_row_0
        rts
