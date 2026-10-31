; 6809 translation of ROM $6F75/$72B4/$795F/$7A81/$7EB9/$8114/$8014.
; Native entity fields: +12 original AI state, +13 original shot counter,
; +15 formation member, +16 target X, +24 target Y, +25 target fraction,
; +26 raw wave flags, +27 AI timer. Renderer fields +17..22 stay separate.
AI_GENERATORS equ $0a00
AI_FORMATIONS equ $0a20
AI_FORM_INDEX equ $0a2c
AI_FORM_X equ $0a2d
AI_TYPE2_X equ $0a2e

ai_init
        ldx #AI_GENERATORS
ai_clear
        clr ,x+
        cmpx #AI_KIND7_BG+1
        blo ai_clear
        clr <wave_left
        clr <wave_wait
        clr <wave_wait+1
        rts

; The generator advances once per 32 MSX frames, not on every native tick.
; Each native tick is the original alternate-frame enemy update (30 Hz).
spawn_waves
        ldd <scroll_steps
        cmpd #24
        lblo ai_generators_return
        lda <ai_clock
        anda #30
        lbne ai_release_generators
        lda <wave_wait+1
        bita #127
        beq ai_wave_ready
        dec <wave_wait+1
        lbra ai_release_generators
ai_wave_ready
        tsta
        bmi ai_wave_find_generator
        ldx #AI_GENERATORS
        tst ,x
        lbne ai_release_generators
        tst 16,x
        lbne ai_release_generators
        ldx #ENTITIES
ai_wave_wait_enemy
        tst ,x
        lbne ai_release_generators
        leax 32,x
        cmpx #ENTITIES+7*32
        blo ai_wave_wait_enemy
ai_wave_find_generator
        ldy #AI_GENERATORS
        tst ,y
        beq ai_wave_load
        leay 16,y
        tst ,y
        lbne ai_release_generators
ai_wave_load
        ldb #16
        tfr y,x
ai_wave_clear
        clr ,x+
        decb
        bne ai_wave_clear
        lda <wave_index
        lsla
        jsr phase_wave_table
        leax a,x
        lda ,x+
        sta ,y
        sta <wave_flags
        anda #15
        ldu #ai_burst_size
        lda a,u
        sta 3,y
        ldb ,x
        tfr b,a
        anda #15
        lsla
        ldb ,y
        bitb #32
        beq ai_wave_wait_ready
        ora #128
ai_wave_wait_ready
        sta <wave_wait+1
        tst ,y
        beq ai_wave_patterns_ready
        inc 8,y
ai_wave_patterns_ready
        lda ,x
        lsra
        lsra
        lsra
        lsra
        sta 6,y
        inc <wave_index
        lda <wave_index
        cmpa PHASE_WAVE_COUNT
        blo ai_release_generators
        clr <wave_index
ai_release_generators
        ; Original stops producing enemies at world tile row $C8.
        ldd <scroll_steps
        cmpd #1600
        bhs ai_generators_return
        ldy #AI_GENERATORS
        jsr ai_release_one
        ldy #AI_GENERATORS+16
        jmp ai_release_one
ai_generators_return
        rts
ai_release_one
        sty <ai_gen_ptr
        tst ,y
        beq ai_generators_return
        tst 8,y
        beq ai_generator_patterns_ready
        clr 8,y
        rts
ai_generator_patterns_ready
        tst 5,y
        bne ai_burst_wait
        tst 1,y
        beq ai_maybe_start_burst
        dec 1,y
        rts
ai_burst_wait
        tst 4,y
        beq ai_burst_ready
        dec 4,y
        rts
ai_burst_ready
        lda ,y
        anda #15
        ldx #ai_burst_gap
        lda a,x
        sta 4,y
        dec 5,y
        bra ai_find_enemy_slot
ai_maybe_start_burst
        lda ,y
        bita #64
        beq ai_find_enemy_slot
        lda 3,y
        sta 5,y
        rts
ai_find_enemy_slot
        ldx #ENTITIES
ai_slot_search
        tst ,x
        beq ai_spawn
        leax 32,x
        cmpx <enemy_limit_end
        blo ai_slot_search
        rts
ai_spawn
        lda #1
        sta ,x
        sta 12,x
        lda ,y
        sta 26,x
        anda #15
        sta 1,x
        sta <ai_kind
        lda #255
        sta 15,x
        lda ,y
        bita #64
        beq ai_spawn_position
        lda AI_FORM_INDEX
        ldb #3
        mul
        ldu #AI_FORMATIONS
        leau d,u
        lda AI_FORM_INDEX
        lsla
        lsla
        lsla
        lsla
        ora ,u
        sta 15,x
        tst ,u
        bne ai_formation_has_x
        lda <player_x
        sta AI_FORM_X
ai_formation_has_x
        inc ,u
        lda 3,y
        cmpa #1
        bne ai_formation_size_ready
        lda #255
ai_formation_size_ready
        sta 1,u
ai_spawn_position
        clrb
        lda <ai_kind
        ldu #ai_spawn_y
        lda a,u
        sta 4,x
        lda 2,y
        anda #7
        ldu #original_spawn_x
        lda a,u
        tst 2,y
        bne ai_spawn_x_ready
        lda <player_x
        ldb ,y
        bitb #64
        beq ai_spawn_choose_side
        lda AI_FORM_X
ai_spawn_choose_side
        cmpa #135
        lda #80
        bhs ai_spawn_x_ready
        lda #176
ai_spawn_x_ready
        clrb
        sta 2,x
        lda <ai_kind
        ldu #original_spawn_gaps
        lda a,u
        sta 1,y
        tst 5,y
        bne ai_spawn_return
        lda ,y
        bita #64
        beq ai_spawn_group_done
        lda AI_FORM_INDEX
        inca
        anda #3
        sta AI_FORM_INDEX
        ldb #3
        mul
        ldu #AI_FORMATIONS
        leau d,u
        clr ,u
        clr 1,u
        clr 2,u
ai_spawn_group_done
        inc 2,y
        lda 2,y
        cmpa 6,y
        blo ai_spawn_return
        clr ,y
        clr 1,y
        clr 2,y
ai_spawn_return
        rts

move_enemies
        ldx #ENTITIES
ai_enemy_next
        stx <entity_ptr
        lda ,x
        beq ai_enemy_next_slot
        cmpa #2
        beq ai_enemy_explosion
        jsr ai_enemy_tick
        bra ai_enemy_next_slot
ai_enemy_explosion
        jsr ai_explosion_tick
ai_enemy_next_slot
        ldx <entity_ptr
        leax 32,x
        cmpx #ENTITIES+7*32
        blo ai_enemy_next
        rts

ai_enemy_tick
        lda 1,x
        cmpa #16
        lbhs ai_tick_return
        lda 12,x
        cmpa #2
        lblo ai_enemy_init
        lda 1,x
        lbeq ai_phase2_dispatch
        cmpa #3
        lbeq ai_phase2_dispatch
        cmpa #4
        lbeq ai_phase2_dispatch
        cmpa #7
        lbeq ai_phase2_dispatch
        cmpa #11
        lbeq ai_phase2_dispatch
        cmpa #1
        beq ai_pull_full
        cmpa #2
        beq ai_pull_half
        cmpa #12
        bne ai_normal_type_tick
        lda 12,x
        cmpa #3
        lbeq ai_twelve_dart
        cmpa #4
        lbeq ai_twelve_fast
        jmp ai_satellite_move
ai_twelve_fast
        jsr ai_animate
        jmp ai_integrate
ai_normal_type_tick
        jsr ai_animate
        jsr ai_enemy_fire
        jsr ai_integrate
        tst ,x
        beq ai_tick_return
        lda 1,x
        cmpa #9
        bne ai_tick_return
        lda 12,x
        cmpa #2
        bne ai_nine_dash
        lda 4,x
        cmpa #192
        bhs ai_tick_return
        lda <player_y
        suba 4,x
        cmpa #64
        bhs ai_tick_return
        ldd #$0120
        tst 2,x
        bmi ai_nine_negative
        bra ai_nine_velocity
ai_nine_negative
        ldd #$fee0
ai_nine_velocity
        std 6,x
        clr 8,x
        clr 9,x
        lda #48
        sta 27,x
        inc 12,x
ai_tick_return
        rts
ai_nine_dash
        dec 27,x
        bne ai_tick_return
        clr 6,x
        clr 7,x
        ldd #$0180
        std 8,x
        rts
ai_pull_full
        ldb 2,x
        subb 16,x
        sex
        bra ai_pull_apply
ai_pull_half
        ldb 2,x
        subb 16,x
        sex
        asrb                    ; Z80 SRA E, D keeps its original sign
ai_pull_apply
        std <ai_temp
        ldd 6,x
        subd <ai_temp
        std 6,x
        jsr ai_animate
        jsr ai_enemy_fire
        jmp ai_integrate
ai_satellite_move
        ldb 4,x
        subb 24,x
        sex
        std <ai_temp
        ldd 8,x
        subd <ai_temp
        std 8,x
        jsr ai_animate
        jsr ai_integrate
        lda 25,x
        adda #32
        sta 25,x
        bcc ai_satellite_target
        inc 24,x
ai_satellite_target
        lda 4,x
        cmpa #96
        bhs ai_tick_return
        lda 2,x
        cmpa #24
        bhs ai_satellite_right
        ldd #$0100
        std 6,x
        rts
ai_satellite_right
        cmpa #232
        lblo ai_tick_return
        ldd #$ff00
        std 6,x
        rts

ai_enemy_init
        clr 6,x
        clr 7,x
        clr 8,x
        clr 9,x
        clr 13,x
        clr 16,x
        clr 24,x
        clr 25,x
        clr 27,x
        lda #1
        sta 10,x
        lda #2
        sta 12,x
        lda 1,x
        cmpa #1
        beq ai_init_one
        cmpa #2
        beq ai_init_two
        cmpa #9
        beq ai_init_nine
        cmpa #12
        beq ai_init_satellite
        cmpa #14
        beq ai_init_fourteen
        jmp ai_phase2_init
ai_init_fourteen
        ldd #$0080
        std 8,x
        jmp ai_init_frame
ai_init_one
        ldd #$0130
        std 8,x
        lda #128
        sta 16,x
        ldu #ai_launch_x
        clrb
ai_one_column
        lda b,u
        cmpa 2,x
        beq ai_one_launch
        incb
        cmpb #6
        blo ai_one_column
        lda #1
        sta 12,x
        bra ai_init_frame
ai_one_launch
        ldu #ai_launch_vx_hi
        lda b,u
        sta 6,x
        bra ai_init_frame
ai_init_two
        ldd #$0200
        std 8,x
        lda <player_x
        ldb 15,x
        cmpb #255
        beq ai_two_select
        bitb #15
        bne ai_two_shared
        sta AI_TYPE2_X
ai_two_shared
        lda AI_TYPE2_X
ai_two_select
        cmpa #128
        blo ai_two_right
        lda #16
        sta 2,x
        lda #32
        sta 16,x
        ldd #$0380
        bra ai_two_velocity
ai_two_right
        lda #240
        sta 2,x
        lda #224
        sta 16,x
        ldd #$fc80
ai_two_velocity
        std 6,x
        bra ai_init_frame
ai_init_nine
        ldd #$0100
        std 8,x
        lda #3
        sta 10,x
        bra ai_init_frame
ai_init_satellite
        ldd #$0080
        std 8,x
        lda #16
        sta 24,x
        lda 2,x
        cmpa #128
        blo ai_satellite_left_init
        lda #240
        ldd #$ff00
        std 6,x
        lda #240
        bra ai_satellite_x_init
ai_satellite_left_init
        ldd #$0100
        std 6,x
        lda #16
ai_satellite_x_init
        sta 2,x
ai_init_frame
        ; Initializers choose the first sprite, without animating or moving.
        lda 1,x
        lsla
        ldu #enemy_art_ids
        lda a,u
        sta 14,x
        rts
        include "ai_phase2.asm"
ai_animate
        lda 1,x
        lsla
        ldu #enemy_art_ids
        leau a,u
        lda <ai_clock
        bita #8
        beq ai_animation_frame
        leau 1,u
ai_animation_frame
        lda ,u
        sta 14,x
        rts
ai_integrate
        ldd 4,x
        addd 8,x
        std 4,x
        cmpa #168
        blo ai_integrate_x
        cmpa #232
        blo ai_remove
ai_integrate_x
        ldd 2,x
        addd 6,x
        std 2,x
        suba #8
        cmpa #240
        lblo ai_tick_return
ai_remove
        clr ,x
        lda #224
        sta 4,x
        rts

ai_enemy_fire
        lda 4,x
        cmpa #192
        bhs ai_fire_return
        lda 13,x
        cmpa #255
        beq ai_fire_return
        inc 13,x
        lda 1,x
        ldu #ai_shot_schedule_counts
        ldb a,u
        decb                    ; first three MSX stages use one fewer event
        lsla
        ldu #ai_shot_schedules
        ldu a,u
        lda 13,x
ai_shot_clock_search
        cmpa ,u+
        beq ai_shot_due
        decb
        bne ai_shot_clock_search
ai_fire_return
        rts
ai_shot_due
        lda <player_y
        adda #8
        suba 4,x
        bcc ai_fire_dy_abs
        nega
ai_fire_dy_abs
        cmpa #24
        blo ai_fire_return
        lda <player_x
        adda #8
        suba 2,x
        bcc ai_fire_dx_abs
        nega
ai_fire_dx_abs
        cmpa #24
        blo ai_fire_return
        lda 26,x
        bpl ai_fire_return
        lda 15,x
        cmpa #255
        beq ai_fire_allowed
        bita #1
        bne ai_fire_return
ai_fire_allowed
        lda 2,x
        sta <aim_x
        lda 4,x
        sta <aim_y
        lda 1,x
        ldu #ai_enemy_burst_kind
        lda a,u
        sta <ai_shot_kind
        cmpa #5
        lbeq phase2_enemy_burst
        cmpa #6
        lbeq phase2_enemy_burst
        jmp aimed_shot

; ROM $84E0: 32 directions, integer quotient and the original tangent table.
; ROM adds 16 to each player coordinate (the two +8 operations are retained).
ai_aim_angle
        clr <ai_quadrant
        lda <player_y
        adda #16
        suba <aim_y
        bcs ai_aim_up
        lda #3
        sta <ai_quadrant
        lda <player_y
        adda #16
        suba <aim_y
        bra ai_aim_y_abs
ai_aim_up
        nega
ai_aim_y_abs
        clrb
        std <ai_quotient
        lda <player_x
        adda #16
        suba <aim_x
        bcc ai_aim_x_abs
        nega
        ldb <ai_quadrant
        eorb #1
        stb <ai_quadrant
ai_aim_x_abs
        sta <ai_divisor
        beq ai_aim_infinite
        clr <ai_remainder
        clr <ai_remainder+1
        lda #16
        sta <ai_count
ai_divide
        lsl <ai_quotient+1
        rol <ai_quotient
        rol <ai_remainder+1
        rol <ai_remainder
        ldd <ai_remainder
        tsta
        bne ai_divide_sub
        cmpb <ai_divisor
        blo ai_divide_next
ai_divide_sub
        subb <ai_divisor
        sbca #0
        std <ai_remainder
        inc <ai_quotient+1
ai_divide_next
        dec <ai_count
        bne ai_divide
        ldd <ai_quotient
        bra ai_angle_search_start
ai_aim_infinite
        ldd #$ffff
        std <ai_quotient
ai_angle_search_start
        ldu #ai_tangents
        lda #64
        sta <ai_count
ai_angle_search
        ldd ,u++
        cmpd <ai_quotient
        blo ai_angle_found
        lda <ai_count
        suba #8
        sta <ai_count
        bne ai_angle_search
ai_angle_found
        lda <ai_quadrant
        beq ai_angle_zero
        cmpa #1
        beq ai_angle_one
        cmpa #2
        beq ai_angle_two
        lda <ai_count
        nega
        rts
ai_angle_one
        lda #128
        suba <ai_count
        rts
ai_angle_two
        lda #128
        adda <ai_count
        rts
ai_angle_zero
        lda <ai_count
        rts

aimed_shot
        pshs x,y
        ldy #ENTITIES+10*32
ai_find_bullet
        tst ,y
        beq ai_new_bullet
        leay 32,y
        cmpy #ENTITIES+18*32
        blo ai_find_bullet
        puls x,y,pc
ai_new_bullet
        jsr ai_aim_angle
        sta <ai_kind
        lsra
        tfr a,b
        clra
        ldu #ai_vectors_8
ai_bullet_vector
        clra
        leau d,u
        ldd ,u
        std 6,y
        ldd 2,u
        std 8,y
        lda <ai_shot_kind
        cmpa #8
        beq ai_bullet_art
        ; Type 1: 3x the sine/cosine. Types 2/3: 2x; type 2 adds
        ; the ROM's unsigned auxiliary cosine to X ($870B..$871C).
        ldd 6,y
        asra
        rorb
        std 6,y
        ldd 8,y
        asra
        rorb
        std 8,y
        lda <ai_shot_kind
        cmpa #1
        bne ai_bullet_type_two
        ldd 6,y
        asra
        rorb
        addd 6,y
        std 6,y
        ldd 8,y
        asra
        rorb
        addd 8,y
        std 8,y
        bra ai_bullet_art
ai_bullet_type_two
        cmpa #2
        bne ai_bullet_art
        lda <ai_kind
        lsra
        lsra
        lsra
        ldu #ai_auxiliary_x
        ldb a,u
        clra
        addd 6,y
        std 6,y
ai_bullet_art
        lda #BULLET_ART
        ldb <ai_shot_kind
        cmpb #8
        beq ai_bullet_art_ready
        lda #SMALL_BULLET_ART
        cmpb #1
        bne ai_bullet_art_ready
        lda <ai_kind
        lsra
        lsra
        lsra
        lsra
        ldu #enemy_bullet1_art
        lda a,u
ai_bullet_art_ready
        sta 14,y
        lda #1
        sta ,y
        lda <ai_shot_kind
        sta 1,y
        lda <aim_x
        clrb
        std 2,y
        lda <aim_y
        std 4,y
        puls x,y,pc

ai_explosion_tick
        dec 11,x
        beq ai_explosion_done
        lda #EXPLOSION_ART
        ldb 11,x
        cmpb #12
        bhi ai_explosion_first
        inca
        cmpb #6
        bhi ai_explosion_second
        inca
        lda <ai_clock
        bita #2
        beq ai_explosion_last
        lda #255
        sta 14,x
        rts
ai_explosion_last
        lda #EXPLOSION_ART+2
        bra ai_explosion_store
ai_explosion_first
        lda #EXPLOSION_ART
ai_explosion_second
        ldb <ai_clock
        bitb #2
        beq ai_explosion_store
        inca
ai_explosion_store
        sta 14,x
        rts
ai_explosion_done
        clr ,x
        lda <combat_status
        cmpa #1
        beq ai_explosion_return
        lda 26,x
        bita #64
        beq ai_explosion_return
        lda 15,x
        lsra
        lsra
        lsra
        lsra
        ldb #3
        mul
        ldu #AI_FORMATIONS
        leau d,u
        inc 2,u
        lda 2,u
        cmpa 1,u
        bne ai_explosion_return
        ldd #1000
        jmp ai_add_score
ai_explosion_return
        rts

ai_add_score
        addd <score
        bcc ai_score_in_range
        ldd #65535
ai_score_in_range
        std <score
        cmpd <high_score
        bls ai_score_done
        std <high_score
ai_score_done
        lda #1
        sta <hud_dirty
        rts

        include "ai_data.inc"
