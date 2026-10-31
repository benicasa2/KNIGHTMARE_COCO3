; ROM $53D3/$549A/$555C: restart the current 24-tile-row (192px) segment.
; Equipment is reset on a lost life; score/lives/session record are retained.
; On segment 9, first-boss position, damage, timer and target are retained.
checkpoint_restore
        jsr sound_stop
        clr <respawn_pending
        ldd #MAP_HEIGHT-VIEW_HEIGHT
        subd <camera_y
        clra
        sta <checkpoint_segment
        ldd #MAP_HEIGHT-VIEW_HEIGHT
        subd <camera_y
checkpoint_find_segment
        cmpd #192
        blo checkpoint_segment_found
        subd #192
        inc <checkpoint_segment
        bra checkpoint_find_segment
checkpoint_segment_found
        addd <camera_y
        std <camera_y
        ; Rewind from the next wave, as $549D..$54B4, to the entry following
        ; the preceding zero-delay entry. Underflow restarts at entry zero.
        ldb <wave_index
        decb
checkpoint_rewind_wave
        decb
        bmi checkpoint_wave_zero
        tfr b,a
        lsla
        jsr phase_wave_table
        leax a,x
        lda 1,x
        anda #15
        bne checkpoint_rewind_wave
        incb
        bra checkpoint_wave_found
checkpoint_wave_zero
        clrb
checkpoint_wave_found
        stb <wave_index
        tst <boss_state
        beq checkpoint_no_satellite_save
        jsr boss_save_satellite_fractions
checkpoint_no_satellite_save
        jsr ai_init
        ldx #ENTITIES
checkpoint_clear_entities
        clr ,x+
        cmpx #ENTITY_END
        blo checkpoint_clear_entities
        clr <boss_drawn
        clr <sprite_valid
        clr <combat_status
        clr <player_invul
        clr <combat_frames
        clr <combat_frames+1
        lda <checkpoint_segment
        cmpa #9
        bne checkpoint_clear_boss
        jsr boss_restore_satellite_fractions
        bra checkpoint_keep_boss
checkpoint_clear_boss
        clr <boss_state
        clr <boss_phase
checkpoint_keep_boss
        jsr items_init
        clr <satellite_side
        ; Skip positional tokens already behind the restored camera.
        jsr phase_token_table
checkpoint_skip_tokens
        lda <token_index
        cmpa PHASE_TOKEN_COUNT
        bhs checkpoint_tokens_ready
        ldd ,x
        cmpd <camera_y
        bls checkpoint_tokens_ready
        leax 3,x
        inc <token_index
        bra checkpoint_skip_tokens
checkpoint_tokens_ready
        lda #1
        sta <hud_dirty
        clr $ff9a
        jmp game_rebuild
