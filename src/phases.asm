; Campaign state lives outside the crowded direct page.
PHASE_WAVE_COUNT equ $0db0
PHASE_FIXED_COUNT equ $0db1
PHASE_TOKEN_COUNT equ $0db2
EXIT_STATE equ $0db3
EXIT_ANGLE equ $0db4
EXIT_VX equ $0db5
EXIT_VY equ $0db7
phase_profile
        lda #WAVE_COUNT
        ldb #FIXED_COUNT
        tst <campaign_stage
        beq phase_profile_ready
        lda #PHASE2_WAVE_COUNT
        ldb #PHASE2_FIXED_COUNT
phase_profile_ready
        sta PHASE_WAVE_COUNT
        stb PHASE_FIXED_COUNT
        lda #9
        sta PHASE_TOKEN_COUNT
        rts
phase_wave_table
        ldx #original_waves
        tst <campaign_stage
        beq phase_table_return
        ldx #phase2_waves
phase_table_return
        rts
phase_fixed_table
        ldx #fixed_item_table
        tst <campaign_stage
        beq phase_table_return
        ldx #phase2_fixed_items
        rts
phase_token_table
        ldx #floating_item_table
        tst <campaign_stage
        beq phase_table_return
        ldx #phase2_tokens
        rts
        include "phase2_data.inc"
