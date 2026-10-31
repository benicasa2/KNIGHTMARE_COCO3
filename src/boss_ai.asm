; First guardian: ROM $8A95..$8CBF. Body logic runs at original 60 Hz;
; three type-12 satellites run once per native/alternate-frame update.
start_boss
        ldx #ENTITIES
boss_clear_entities
        clr ,x
        leax 32,x
        cmpx #ENTITY_END
        blo boss_clear_entities
        jsr ai_init
        jsr boss_restore_satellite_fractions
        lda #1
        sta <boss_state
        sta <boss_phase
        tst <campaign_stage
        beq boss_start_first
        lda #2
        sta <boss_phase
boss_start_first
        lda #BOSS_HITS
        sta <boss_hp
        tst <campaign_stage
        beq boss_start_position
        lda #30
        sta <boss_hp
boss_start_position
        lda #112
        sta <boss_x
        tst <campaign_stage
        beq boss_start_y
        lda #120
        sta <boss_x
boss_start_y
        lda #216
        sta <boss_y
        ldd <combat_frames
        lslb
        rola
        subd #2
        std <boss_clock
        ldd #3600
        std <boss_lifetime
        clr <boss_wait
        clr <satellite_side
        rts

; Original satellite records live outside the ordinary enemy pool. Preserve
; their fractional positions separately when the native pool is reused.
boss_save_satellite_fractions
        ldx #ENTITIES
        ldu #$0a40
        ldb #3
boss_save_fraction
        lda 3,x
        sta ,u+
        lda 5,x
        sta ,u+
        leax 32,x
        decb
        bne boss_save_fraction
        rts
boss_restore_satellite_fractions
        ldx #ENTITIES
        ldu #$0a40
        ldb #3
boss_restore_fraction
        lda ,u+
        sta 3,x
        lda ,u+
        sta 5,x
        leax 32,x
        decb
        bne boss_restore_fraction
        rts

update_boss
        tst <campaign_stage
        lbne boss2_update
        clr <ai_substep
boss_original_frame
        ldd <boss_clock
        addd #1
        std <boss_clock
        lda <boss_state
        cmpa #2
        beq boss_death_frame
        cmpa #1
        lbne boss_frame_done
        tst <boss_phase
        beq boss_fight_frame
        lda <boss_clock+1
        anda #63
        cmpa #1
        lbne boss_frame_done
        lda <boss_y
        adda #8
        sta <boss_y
        lbmi boss_frame_done
        cmpa #32
        lblo boss_frame_done
        clr <boss_phase
        jsr boss_choose_target
        lbra boss_frame_done
boss_death_frame
        dec <boss_wait
        lbne boss_frame_done
        lda #3
        sta <boss_state
        sta <combat_status
        lda #1
        sta <unlocked_exit
        lda #$12
        sta $ff9a
        lbra boss_frame_done
boss_fight_frame
        ldd <boss_lifetime
        subd #1
        std <boss_lifetime
        lbeq boss_begin_death
        lda <ai_clock
        adda <ai_substep
        anda #31
        bne boss_movement
        lda <boss_x
        adda #10
        sta <aim_x
        lda <boss_y
        adda #12
        sta <aim_y
        lda #8
        sta <ai_shot_kind
        jsr aimed_shot
boss_movement
        tst <boss_wait
        beq boss_move_to_target
        dec <boss_wait
        lbne boss_frame_done
        jsr boss_choose_target
        lbra boss_frame_done
boss_move_to_target
        lda <boss_x
        anda #252
        adda #12
        cmpa <boss_target
        beq boss_stop_at_target
        lda <boss_x
        adda <boss_velocity
        cmpa #32
        blo boss_clamp_left
        cmpa #191
        bhi boss_clamp_right
        sta <boss_x
        lbra boss_frame_done
boss_clamp_left
        lda #32
        bra boss_clamped
boss_clamp_right
        lda #191
boss_clamped
        sta <boss_x
boss_stop_at_target
        lda #80
        sta <boss_wait
boss_frame_done
        inc <ai_substep
        lda <ai_substep
        cmpa #2
        lblo boss_original_frame
        lda <boss_state
        cmpa #1
        bne boss_update_return
        tst <boss_phase
        bne boss_update_return
        ldx #ENTITIES
        ldu #satellite_clocks
        clr <ai_satellite_index
boss_satellite_clock
        inc ,u
        ldb <ai_satellite_index
        ldy #ai_satellite_thresholds
        lda b,y
        cmpa ,u+
        bne boss_next_satellite
        tst ,x
        bne boss_next_satellite
        lda #1
        sta ,x
        sta 12,x
        sta 10,x
        lda #12
        sta 1,x
        lda #16
        clrb
        sta 4,x
        inc <satellite_side
        lda <satellite_side
        anda #1
        adda #127
        sta 2,x
        lda #255
        sta 15,x
        clr 26,x
boss_next_satellite
        leax 32,x
        inc <ai_satellite_index
        lda <ai_satellite_index
        cmpa #3
        blo boss_satellite_clock
boss_update_return
        rts
boss_choose_target
        lda <player_x
        anda #252
        sta <boss_target
        lda <boss_x
        anda #252
        adda #12
        cmpa <boss_target
        beq boss_target_zero
        lda #1
        blo boss_target_velocity
        lda #255
        bra boss_target_velocity
boss_target_zero
        clra
boss_target_velocity
        sta <boss_velocity
        rts
boss_begin_death
        jsr sfx_boss
        lda #2
        sta <boss_state
        lda #208                ; $40 then three $30-frame death states
        sta <boss_wait
        ldx #ENTITIES
boss_defeat_clear
        clr ,x
        leax 32,x
        cmpx #ENTITY_END
        blo boss_defeat_clear
        rts

hit_boss
        tst <campaign_stage
        lbne boss2_hit
        lda <boss_state
        cmpa #1
        bne hit_boss_return
        tst <boss_phase
        bne hit_boss_return
        ldy #ENTITIES+7*32
boss_hit_arrow
        tst ,y
        beq boss_next_arrow
        lda 1,y
        lsla
        ldu #ai_boss_weapon_boxes
        leau a,u
        lda ,u
        suba #14
        sta <ai_boss_adjust
        adda #26
        sta <ai_boss_width
        lda 2,y
        suba <boss_x
        adda <ai_boss_adjust
        cmpa <ai_boss_width
        bhs boss_next_arrow
        lda 4,y
        suba <boss_y
        suba #8
        cmpa #16
        bhs boss_next_arrow
        clr ,y
        jsr sfx_hit
        lda 1,u
        sta <ai_boss_adjust
        lda <boss_hp
        suba <ai_boss_adjust
        bls boss_killed_by_shot
        sta <boss_hp
        bra boss_next_arrow
boss_killed_by_shot
        clr <boss_hp
        jsr boss_begin_death
        ldd #10000
        jmp ai_add_score
boss_next_arrow
        leay 32,y
        cmpy #ENTITIES+10*32
        blo boss_hit_arrow
hit_boss_return
        rts

        include "boss2.asm"
