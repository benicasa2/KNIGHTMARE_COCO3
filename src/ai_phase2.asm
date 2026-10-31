; Additional enemy state machines translated from $7AD2/$7BC7/$7C33/
; $7DC5/$7FE5/$8076. Positions/velocities remain native 8.8 fixed point.
AI_KIND7_BG equ $0a2f
ai_phase2_dispatch
        lda 1,x
        beq ai_zero_tick
        cmpa #3
        lbeq ai_three_tick
        cmpa #4
        lbeq ai_four_tick
        cmpa #7
        lbeq ai_seven_tick
        cmpa #11
        lbeq ai_eleven_tick
        rts
ai_phase2_init
        lda 1,x
        beq ai_zero_init
        cmpa #3
        lbeq ai_three_init
        cmpa #4
        lbeq ai_four_init
        cmpa #7
        lbeq ai_seven_init
        cmpa #11
        lbeq ai_eleven_init
        clr ,x
        rts
ai_zero_init
        ldd #$0600
        std 8,x
        ldd #$0200
        lda 2,x
        cmpa #128
        ldd #$0200
        blo ai_zero_velocity
        ldd #$fe00
ai_zero_velocity
        std 6,x
        lda phase2_art_0
        sta 14,x
        rts
ai_zero_tick
        lda 12,x
        cmpa #2
        beq ai_zero_approach
        cmpa #5
        lbeq ai_integrate
        lda <ai_clock
        anda #8
        lsra
        lsra
        lsra
        inca
        ldu #phase2_art_0
        lda a,u
        sta 14,x
        dec 27,x
        bne ai_zero_return
        lda 12,x
        cmpa #3
        bne ai_zero_leave
        jsr ai_shot_due
        lda #8
        sta 27,x
        inc 12,x
        rts
ai_zero_leave
        ldd #$fa00
        std 8,x
        lda phase2_art_0
        sta 14,x
        inc 12,x
ai_zero_return
        rts
ai_zero_approach
        jsr ai_integrate
        lda 4,x
        cmpa #8
        blo ai_zero_return
        cmpa #192
        bhs ai_zero_return
        lda <player_y
        suba 4,x
        cmpa #80
        bhs ai_zero_return
        lda #32
        sta 27,x
        inc 12,x
        rts
ai_three_init
        ldd #$ffc0
        std 8,x
        lda #48
        sta 24,x
        lda #128
        sta 16,x
        lda 2,x
        cmpa #128
        bhs ai_three_right
        lda #16
        sta 2,x
        ldd #$0300
        bra ai_three_speed
ai_three_right
        lda #240
        sta 2,x
        ldd #$fd00
ai_three_speed
        std 6,x
        jmp ai_init_frame
ai_three_tick
        lda 12,x
        cmpa #2
        bne ai_three_springs
        jsr ai_animate
        jsr ai_enemy_fire
        jsr ai_integrate
        lda 2,x
        suba #128
        bcc ai_three_abs
        nega
ai_three_abs
        cmpa #5
        bhs ai_zero_return
        clr 8,x
        clr 9,x
        inc 12,x
        rts
ai_three_springs
        ldb 4,x
        subb 24,x
        sex
        asrb
        std <ai_temp
        ldd 8,x
        subd <ai_temp
        std 8,x
        jmp ai_pull_half
ai_four_init
        ldd #$0250
        std 8,x
        jmp ai_init_frame
ai_four_tick
        jsr ai_animate
        jsr ai_enemy_fire
        jsr ai_integrate
        lda 12,x
        cmpa #2
        bne ai_four_return
        lda 4,x
        cmpa #8
        lblo ai_four_return
        cmpa #192
        lbhs ai_four_return
        lda <player_y
        suba 4,x
        cmpa #80
        lbhs ai_four_return
        lda <player_x
        cmpa 2,x
        ldd #$027f
        bhs ai_four_speed
        ldd #$fd81
ai_four_speed
        std 6,x
        inc 12,x
ai_four_return
        rts
ai_seven_init
        lda #4
        sta 10,x
        clr 24,x
        clr 25,x
        lda AI_KIND7_BG
        eora #1
        sta AI_KIND7_BG
        sta 28,x
        clr 16,x
        lda 2,x
        cmpa #128
        lda #32
        blo ai_seven_position
        lda #1
        sta 16,x
        lda #224
ai_seven_position
        sta 2,x
        jmp ai_init_frame
ai_seven_tick
        lda 12,x
        cmpa #2
        beq ai_seven_speed
        cmpa #3
        beq ai_seven_cross
        cmpa #4
        lbeq ai_seven_hit
        dec 27,x
        bne ai_four_return
        cmpa #5
        lbeq ai_seven_expand
        cmpa #6
        lbeq ai_seven_recover
        dec 29,x
        jsr ai_seven_art
        lda #6
        sta 27,x
        dec 25,x
        bne ai_four_return
        inc 24,x
        lda 24,x
        cmpa #4
        blo ai_seven_lap_ready
        lda #4
        sta 24,x
ai_seven_lap_ready
        lda #2
        sta 12,x
        rts
ai_seven_speed
        ldu #phase2_seven_vx_left
        tst 16,x
        beq ai_seven_speed_side
        ldu #phase2_seven_vx_right
ai_seven_speed_side
        lda 24,x
        lsla
        ldd a,u
        std 6,x
        lda 24,x
        lsla
        ldu #phase2_seven_vy
        ldd a,u
        std 8,x
        inc 12,x
        rts
ai_seven_cross
        jsr ai_animate
        jsr ai_enemy_fire
        jsr ai_integrate
        tst 16,x
        bne ai_seven_left_edge
        lda 2,x
        cmpa #224
        lblo ai_four_return
        bra ai_seven_turn
ai_seven_left_edge
        lda 2,x
        cmpa #33
        lbhs ai_four_return
ai_seven_turn
        lda 16,x
        eora #1
        sta 16,x
        dec 12,x
        rts
ai_seven_hit
        ldd #100
        jsr ai_add_score
        lda #2
        sta 29,x
        sta 25,x
        lda #6
        sta 27,x
        inc 12,x
        bra ai_seven_art
ai_seven_expand
        inc 29,x
        jsr ai_seven_art
        lda #6
        sta 27,x
        dec 25,x
        lbne ai_four_return
        lda #32
        sta 27,x
        inc 12,x
        rts
ai_seven_recover
        lda #4
        sta 29,x
        lda #2
        sta 25,x
        lda #6
        sta 27,x
        inc 12,x
        jsr sfx_cycle
ai_seven_art
        lda 29,x
        ldu #phase2_art_7
        lda a,u
        sta 14,x
        rts
ai_eleven_init
        lda <player_y
        pshs a
        lda <ai_clock
        lsra
        lsra
        anda #3
        ldu #phase2_eleven_targets
        lda a,u
        suba #8
        sta <player_y
        jsr ai_phase2_dart
        puls a
        sta <player_y
        jmp ai_init_frame
ai_eleven_tick
        jsr ai_enemy_fire
        jmp ai_integrate
ai_twelve_dart
        jsr ai_phase2_dart
        inc 12,x
        rts
ai_phase2_dart
        lda 2,x
        sta <aim_x
        lda 4,x
        sta <aim_y
        jsr ai_aim_angle
        lsra
        tfr a,b
        clra
        ldu #ai_vectors_8
        leau d,u
        ldd ,u
        std <ai_temp
        asra
        rorb
        asra
        rorb
        addd <ai_temp
        std 6,x
        ldd 2,u
        lslb
        rola
        std 8,x
        rts
phase2_seven_vx_left fdb $0180,$0200,$0280,$0300,$fe80
phase2_seven_vx_right fdb $fe80,$fe00,$fd80,$fd00,$0060
phase2_seven_vy fdb $0060,$0090,$00c0,$0100,$11c6
phase2_eleven_targets fcb $b0,$90,$70,$50


P2_SHOT_ANGLE equ $0daa
P2_SHOTS_LEFT equ $0dab
phase2_enemy_burst
        pshs x,y
        lda #1
        ldb <ai_shot_kind
        cmpb #5
        bne phase2_count_shots
        lda #4
phase2_count_shots
        sta P2_SHOTS_LEFT
        clrb
        ldy #ENTITIES+10*32
phase2_count_free
        tst ,y
        bne phase2_count_next
        incb
phase2_count_next
        leay 32,y
        cmpy #ENTITIES+18*32
        blo phase2_count_free
        cmpb P2_SHOTS_LEFT
        blo phase2_burst_done
        jsr ai_aim_angle
        ldb <ai_shot_kind
        cmpb #5
        bne phase2_angle_ready
        suba #24
phase2_angle_ready
        sta P2_SHOT_ANGLE
phase2_emit_next
        lda <ai_shot_kind
        cmpa #5
        bne phase2_emit_angle
        lda P2_SHOT_ANGLE
        adda #16
        sta P2_SHOT_ANGLE
phase2_emit_angle
        ldy #ENTITIES+10*32
phase2_free_bullet
        tst ,y
        beq phase2_emit_bullet
        leay 32,y
        bra phase2_free_bullet
phase2_emit_bullet
        lda P2_SHOT_ANGLE
        anda #$f8
        lsra
        tfr a,b
        clra
        ldu #phase2_vectors_5
        lda <ai_shot_kind
        cmpa #5
        beq phase2_vector_ready
        ldu #phase2_vectors_6
phase2_vector_ready
        clra
        leau d,u
        ldd ,u
        std 6,y
        ldd 2,u
        std 8,y
        lda #BULLET4_ART
        ldb <ai_shot_kind
        cmpb #5
        beq phase2_bullet_art
        lda #BULLET6_ART
phase2_bullet_art
        sta 14,y
        stb 1,y
        lda #1
        sta ,y
        clr 28,y
        lda <aim_x
        clrb
        std 2,y
        lda <aim_y
        std 4,y
        dec P2_SHOTS_LEFT
        bne phase2_emit_next
phase2_burst_done
        puls x,y,pc
