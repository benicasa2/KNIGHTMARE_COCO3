; Packed-color sprites; zero nibbles are transparent.
raw_row_table
        fdb raw_row_0,raw_row_1,raw_row_2,raw_row_3,raw_row_4,raw_row_5,raw_row_6,raw_row_7,raw_row_8,raw_row_9,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row,raw_wide_row
raw_row_9
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_byte_9_empty
        bitb #$f0
        beq raw_byte_9_keep_hi
        bitb #$0f
        beq raw_byte_9_keep_lo
        stb ,u+
        bra raw_row_8
raw_byte_9_keep_hi
        anda #$f0
        bra raw_byte_9_combine
raw_byte_9_keep_lo
        anda #$0f
raw_byte_9_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_row_8
raw_byte_9_empty
        leau 1,u

raw_row_8
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_byte_8_empty
        bitb #$f0
        beq raw_byte_8_keep_hi
        bitb #$0f
        beq raw_byte_8_keep_lo
        stb ,u+
        bra raw_row_7
raw_byte_8_keep_hi
        anda #$f0
        bra raw_byte_8_combine
raw_byte_8_keep_lo
        anda #$0f
raw_byte_8_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_row_7
raw_byte_8_empty
        leau 1,u

raw_row_7
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_byte_7_empty
        bitb #$f0
        beq raw_byte_7_keep_hi
        bitb #$0f
        beq raw_byte_7_keep_lo
        stb ,u+
        bra raw_row_6
raw_byte_7_keep_hi
        anda #$f0
        bra raw_byte_7_combine
raw_byte_7_keep_lo
        anda #$0f
raw_byte_7_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_row_6
raw_byte_7_empty
        leau 1,u

raw_row_6
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_byte_6_empty
        bitb #$f0
        beq raw_byte_6_keep_hi
        bitb #$0f
        beq raw_byte_6_keep_lo
        stb ,u+
        bra raw_row_5
raw_byte_6_keep_hi
        anda #$f0
        bra raw_byte_6_combine
raw_byte_6_keep_lo
        anda #$0f
raw_byte_6_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_row_5
raw_byte_6_empty
        leau 1,u

raw_row_5
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_byte_5_empty
        bitb #$f0
        beq raw_byte_5_keep_hi
        bitb #$0f
        beq raw_byte_5_keep_lo
        stb ,u+
        bra raw_row_4
raw_byte_5_keep_hi
        anda #$f0
        bra raw_byte_5_combine
raw_byte_5_keep_lo
        anda #$0f
raw_byte_5_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_row_4
raw_byte_5_empty
        leau 1,u

raw_row_4
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_byte_4_empty
        bitb #$f0
        beq raw_byte_4_keep_hi
        bitb #$0f
        beq raw_byte_4_keep_lo
        stb ,u+
        bra raw_row_3
raw_byte_4_keep_hi
        anda #$f0
        bra raw_byte_4_combine
raw_byte_4_keep_lo
        anda #$0f
raw_byte_4_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_row_3
raw_byte_4_empty
        leau 1,u

raw_row_3
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_byte_3_empty
        bitb #$f0
        beq raw_byte_3_keep_hi
        bitb #$0f
        beq raw_byte_3_keep_lo
        stb ,u+
        bra raw_row_2
raw_byte_3_keep_hi
        anda #$f0
        bra raw_byte_3_combine
raw_byte_3_keep_lo
        anda #$0f
raw_byte_3_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_row_2
raw_byte_3_empty
        leau 1,u

raw_row_2
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_byte_2_empty
        bitb #$f0
        beq raw_byte_2_keep_hi
        bitb #$0f
        beq raw_byte_2_keep_lo
        stb ,u+
        bra raw_row_1
raw_byte_2_keep_hi
        anda #$f0
        bra raw_byte_2_combine
raw_byte_2_keep_lo
        anda #$0f
raw_byte_2_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_row_1
raw_byte_2_empty
        leau 1,u

raw_row_1
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_byte_1_empty
        bitb #$f0
        beq raw_byte_1_keep_hi
        bitb #$0f
        beq raw_byte_1_keep_lo
        stb ,u+
        bra raw_row_0
raw_byte_1_keep_hi
        anda #$f0
        bra raw_byte_1_combine
raw_byte_1_keep_lo
        anda #$0f
raw_byte_1_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_row_0
raw_byte_1_empty
        leau 1,u

raw_row_0
        rts
raw_wide_row
        lda <span_n
        sta <span_blocks
raw_wide_next
        lda ,u
        sta ,y+
        ldb ,x+
        beq raw_wide_byte_empty
        bitb #$f0
        beq raw_wide_byte_keep_hi
        bitb #$0f
        beq raw_wide_byte_keep_lo
        stb ,u+
        bra raw_wide_done
raw_wide_byte_keep_hi
        anda #$f0
        bra raw_wide_byte_combine
raw_wide_byte_keep_lo
        anda #$0f
raw_wide_byte_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_wide_done
raw_wide_byte_empty
        leau 1,u

raw_wide_done
        dec <span_blocks
        bne raw_wide_next
        rts
raw_token_row
        lda <span_n
        sta <span_blocks
raw_token_next
        lda ,u
        sta ,y+
        ldb ,x+
        stb <token_byte
        andb #$f0
        cmpb #$e0
        bne raw_token_low
        ldb <token_byte
        andb #15
        orb <token_ink_hi
        stb <token_byte
raw_token_low
        ldb <token_byte
        andb #15
        cmpb #14
        bne raw_token_ready
        ldb <token_byte
        andb #$f0
        orb <token_ink
        stb <token_byte
raw_token_ready
        ldb <token_byte
        beq raw_token_byte_empty
        bitb #$f0
        beq raw_token_byte_keep_hi
        bitb #$0f
        beq raw_token_byte_keep_lo
        stb ,u+
        bra raw_token_done
raw_token_byte_keep_hi
        anda #$f0
        bra raw_token_byte_combine
raw_token_byte_keep_lo
        anda #$0f
raw_token_byte_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_token_done
raw_token_byte_empty
        leau 1,u

raw_token_done
        dec <span_blocks
        bne raw_token_next
        rts

raw_boss_row
        lda <span_n
        sta <span_blocks
raw_boss_next
        lda ,u
        sta ,y+
        ldb ,x+
        stb <token_byte
        andb #$f0
        cmpb #$f0
        bne raw_boss_low
        ldb <token_byte
        andb #15
        orb B2_COLOR_HI
        stb <token_byte
raw_boss_low
        ldb <token_byte
        andb #15
        cmpb #15
        bne raw_boss_ready
        ldb <token_byte
        andb #$f0
        orb B2_COLOR
        stb <token_byte
raw_boss_ready
        ldb <token_byte
        beq raw_boss_byte_empty
        bitb #$f0
        beq raw_boss_byte_keep_hi
        bitb #$0f
        beq raw_boss_byte_keep_lo
        stb ,u+
        bra raw_boss_done
raw_boss_byte_keep_hi
        anda #$f0
        bra raw_boss_byte_combine
raw_boss_byte_keep_lo
        anda #$0f
raw_boss_byte_combine
        stb <token_byte
        ora <token_byte
        sta ,u+
        bra raw_boss_done
raw_boss_byte_empty
        leau 1,u

raw_boss_done
        dec <span_blocks
        bne raw_boss_next
        rts
