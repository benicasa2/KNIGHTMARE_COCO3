; Original $5D95..$5DF7: vector to (120,32), then Y velocity $FF50.
cutscene_tick
        clr <ai_substep
cutscene_frame
        lda EXIT_STATE
        beq cutscene_initialize
        cmpa #3
        lbhs cutscene_done
        lda <player_x
        ldb <player_fx
        addd EXIT_VX
        sta <player_x
        stb <player_fx
        lda <player_y
        ldb <player_fy
        addd EXIT_VY
        sta <player_y
        stb <player_fy
        lda <ai_clock
        adda <ai_substep
        lsra
        lsra
        lsra
        lsra
        anda #1
        sta <player_frame
        lda EXIT_STATE
        cmpa #2
        beq cutscene_in_passage
        lda <player_y
        cmpa #32
        blo cutscene_enter_passage
        lda EXIT_ANGLE
        anda #127
        bne cutscene_next_frame
        lda <player_x
        cmpa #120
        bne cutscene_next_frame
cutscene_enter_passage
        lda #2
        sta EXIT_STATE
        ldd #0
        std EXIT_VX
        ldd #$ff50
        std EXIT_VY
        bra cutscene_next_frame
cutscene_in_passage
        lda <player_y
        cmpa #5
        bhs cutscene_next_frame
        lda #3
        sta EXIT_STATE
        bra cutscene_done
cutscene_initialize
        jsr cutscene_choose_angle
        sta EXIT_ANGLE
        ldb #4
        mul
        ldx #cutscene_vectors
        leax d,x
        ldd ,x
        std EXIT_VX
        ldd 2,x
        std EXIT_VY
        lda EXIT_ANGLE
        anda #127
        cmpa #5
        blo cutscene_flat
        cmpa #123
        blo cutscene_vector_ready
cutscene_flat
        clr EXIT_ANGLE
        clr EXIT_VY
        clr EXIT_VY+1
cutscene_vector_ready
        lda #1
        sta EXIT_STATE
        jsr sfx_pickup
cutscene_next_frame
        inc <ai_substep
        lda <ai_substep
        cmpa #2
        lblo cutscene_frame
cutscene_done
        rts
cutscene_choose_angle
        lda <player_y
        suba #32
        clrb
        std <ai_quotient
        clr <ai_quadrant
        lda #120
        suba <player_x
        bcc cutscene_dx_ready
        nega
        inc <ai_quadrant
cutscene_dx_ready
        sta <ai_divisor
        beq cutscene_infinite
        clr <ai_remainder
        clr <ai_remainder+1
        lda #16
        sta <ai_count
cutscene_divide
        lsl <ai_quotient+1
        rol <ai_quotient
        rol <ai_remainder+1
        rol <ai_remainder
        ldd <ai_remainder
        tsta
        bne cutscene_divide_subtract
        cmpb <ai_divisor
        blo cutscene_divide_next
cutscene_divide_subtract
        subb <ai_divisor
        sbca #0
        std <ai_remainder
        inc <ai_quotient+1
cutscene_divide_next
        dec <ai_count
        bne cutscene_divide
        bra cutscene_angle_search
cutscene_infinite
        ldd #$ffff
        std <ai_quotient
cutscene_angle_search
        ldu #cutscene_tangents
        lda #64
        sta <ai_count
cutscene_search
        ldd ,u++
        cmpd <ai_quotient
        blo cutscene_angle_found
        dec <ai_count
        bne cutscene_search
cutscene_angle_found
        lda <ai_count
        tst <ai_quadrant
        beq cutscene_angle_return
        lda #128
        suba <ai_count
cutscene_angle_return
        rts

PHASE_PERSIST equ $0a80
phase_advance
        tst <campaign_stage
        lbeq phase_advance_two
        ; This build ends after phase 2; do not substitute a fake phase 3.
        jmp game_over_to_intro
phase_advance_two
        ldd <score
        std PHASE_PERSIST
        ldd <high_score
        std PHASE_PERSIST+2
        lda <player_lives
        sta PHASE_PERSIST+4
        lda <weapon_kind
        sta PHASE_PERSIST+5
        lda <speed_level
        sta PHASE_PERSIST+6
        lda <ability_kind
        sta PHASE_PERSIST+7
        ldd <ability_timer
        std PHASE_PERSIST+8
        lda <shield_hits
        sta PHASE_PERSIST+10
        ldd <freeze_ticks
        std PHASE_PERSIST+11
        lda <test_invincible
        sta PHASE_PERSIST+13
        lda <music_enabled
        sta PHASE_PERSIST+14
        lda <input_mode
        sta PHASE_PERSIST+15
        lda #1
        sta <campaign_stage
        jsr phase_profile
        jsr level_tiles_load
        tst FD_ERROR
        lbne game_disk_error
        jsr combat_init
        clr <unlocked_exit
        jsr items_init
        ldd PHASE_PERSIST
        std <score
        ldd PHASE_PERSIST+2
        std <high_score
        lda PHASE_PERSIST+4
        cmpa #99
        bhs phase_lives_ready
        inca
phase_lives_ready
        sta <player_lives
        lda PHASE_PERSIST+5
        sta <weapon_kind
        lda PHASE_PERSIST+6
        sta <speed_level
        lda PHASE_PERSIST+7
        sta <ability_kind
        ldd PHASE_PERSIST+8
        std <ability_timer
        lda PHASE_PERSIST+10
        sta <shield_hits
        ldd PHASE_PERSIST+11
        std <freeze_ticks
        lda PHASE_PERSIST+13
        sta <test_invincible
        lda PHASE_PERSIST+14
        sta <music_enabled
        lda PHASE_PERSIST+15
        sta <input_mode
        jsr campaign_map_load
        jsr phase_music_restart
        ldd #MAP_HEIGHT-VIEW_HEIGHT
        std <camera_y
        jsr game_rebuild
        lda <test_invincible
        beq phase_border_normal
        lda palette+4
phase_border_normal
        sta $ff9a
        rts
phase_music_restart
        pshs cc
        orcc #$50
        jsr sound_stop
        tst <music_enabled
        beq phase_music_return
        ldd #ENTITIES+5*32
        std <enemy_limit_end
        lda #255
        sta MUSIC_TRACK
        jsr music_tick
        lda #$20
        sta $ff93
phase_music_return
        puls cc,pc

        include "phase2_math.inc"
