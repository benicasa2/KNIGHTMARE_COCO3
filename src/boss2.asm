; Second guardian: $8E34..$9090. Two original frame steps per native tick.
B2_XFRAC equ $0dbc
B2_YFRAC equ $0dbd
B2_VX equ $0dbe
B2_VY equ $0dc0
B2_SHOT equ $0dc2
B2_COLOR equ $0dc3
boss2_update
        clr <ai_substep
boss2_frame
        ldd <boss_clock
        addd #1
        std <boss_clock
        lda <boss_state
        cmpa #2
        lbeq boss2_death_frame
        cmpa #1
        lbne boss2_frame_done
        lda <boss_phase
        beq boss2_fight
        cmpa #1
        beq boss2_initialize_motion
        lda <ai_clock
        adda <ai_substep
        anda #63
        cmpa #1
        lbne boss2_frame_done
        lda <boss_y
        adda #8
        sta <boss_y
        lbmi boss2_frame_done
        cmpa #32
        lblo boss2_frame_done
        lda #1
        sta <boss_phase
        lbra boss2_frame_done
boss2_initialize_motion
        clr <boss_phase
        clr B2_XFRAC
        clr B2_YFRAC
        clr B2_SHOT
        ldd #$0180
        std B2_VX
        ldd #$0100
        std B2_VY
        ldd #3600
        std <boss_lifetime
        lbra boss2_frame_done
boss2_fight
        ldd <boss_lifetime
        subd #1
        std <boss_lifetime
        lbeq boss2_begin_death
        jsr boss2_move
        jsr boss2_move_drops
        lda <ai_clock
        adda <ai_substep
        anda #15
        bne boss2_frame_done
        jsr boss2_spawn_drop
boss2_frame_done
        inc <ai_substep
        lda <ai_substep
        cmpa #2
        lblo boss2_frame
        ; Three non-colliding black ornaments, as $9049 (pattern F8, color 1).
        lda <boss_state
        cmpa #1
        bne boss2_return
        ldx #ENTITIES
        ldu #boss2_ball_art
        ldb #0
boss2_ball_loop
        lda #1
        sta ,x
        lda #16
        sta 1,x
        lda ,u+
        sta 14,x
        stb <temp_index
        lda <boss_x
        suba #6
        pshs a
        lda #6
        mul
        addb ,s+
        stb 2,x
        lda #72
        tst <boss_phase
        beq boss2_ball_y
        lda <boss_y
        adda #40
boss2_ball_y
        sta 4,x
        ldb <temp_index
        incb
        leax 32,x
        cmpb #3
        blo boss2_ball_loop
boss2_return
        rts
boss2_death_frame
        dec <boss_wait
        bne boss2_frame_done
        lda #3
        sta <boss_state
        sta <combat_status
        clr EXIT_STATE
        lda #1
        sta <unlocked_exit
        bra boss2_frame_done
boss2_begin_death
        lda <boss_y
        suba #16
        sta <boss_y
        jsr boss_begin_death
        lbra boss2_frame_done
boss2_move
        ldb #32
        subb <boss_y
        sex
        lslb
        rola
        lslb
        rola
        lslb
        rola
        addd B2_VY
        std B2_VY
        lda <boss_y
        ldb B2_YFRAC
        addd B2_VY
        sta <boss_y
        stb B2_YFRAC
        ldb #120
        subb <boss_x
        bmi boss2_negative_pull
        lsrb
        lsrb
        lsrb
        clra
        bra boss2_x_pull
boss2_negative_pull
        negb
        lsrb
        lsrb
        lsrb
        negb
        sex
boss2_x_pull
        addd B2_VX
        std B2_VX
        lda <boss_x
        ldb B2_XFRAC
        addd B2_VX
        sta <boss_x
        stb B2_XFRAC
        rts
boss2_spawn_drop
        ldx #ENTITIES+10*32
boss2_find_drop
        tst ,x
        beq boss2_new_drop
        leax 32,x
        cmpx #ENTITIES+15*32
        blo boss2_find_drop
        rts
boss2_new_drop
        lda #1
        sta ,x
        sta 28,x
        lda #6
        sta 1,x
        lda <boss_y
        adda #16
        sta 4,x
        lda <boss_x
        adda #4
        sta 2,x
        sta 25,x
        lda #100
        sta 24,x
        clr 26,x
        clr 8,x
        clr 9,x
        lda B2_SHOT
        bita #2
        beq boss2_drop_negative
        ldd #$0180
        bra boss2_drop_speed
boss2_drop_negative
        ldd #$fe80
boss2_drop_speed
        std 6,x
        inc B2_SHOT
        lda boss2_drop_art
        sta 14,x
        rts
boss2_move_drops
        ldx #ENTITIES+10*32
boss2_drop_loop
        tst ,x
        lbeq boss2_next_drop
        lda 24,x
        suba 4,x
        bne boss2_drop_y_distance
        inc 26,x
boss2_drop_y_distance
        asra
        asra
        tfr a,b
        sex
        addd 8,x
        std 8,x
        clra
        tst 26,x
        bmi boss2_drop_x_distance
        lda 25,x
        suba 2,x
        bne boss2_drop_x_distance
        lda 26,x
        cmpa #2
        blo boss2_drop_zero_x
        ora #$80
        sta 26,x
boss2_drop_zero_x
        clra
boss2_drop_x_distance
        asra
        asra
        tfr a,b
        sex
        addd 6,x
        std 6,x
        lda <ai_clock
        adda <ai_substep
        anda #12
        lsra
        lsra
        ldu #boss2_drop_art
        lda a,u
        sta 14,x
        jsr ai_integrate
        tst ,x
        beq boss2_next_drop
        ; Supplied ROM $8FD6 is 04 08 (INC B / EX AF,AF'), not LD B,8.
        ; The previous loop leaves B=0: only its first slot gets this test.
        cmpx #ENTITIES+10*32
        bne boss2_next_drop
        tst 8,x
        bpl boss2_next_drop
        lda <boss_y
        suba 4,x
        adda #24
        cmpa #56
        bhs boss2_next_drop
        lda <boss_x
        suba 2,x
        adda #48
        cmpa #80
        bhs boss2_next_drop
        clr ,x
        lda #224
        sta 4,x
boss2_next_drop
        leax 32,x
        cmpx #ENTITIES+15*32
        lblo boss2_drop_loop
        rts

boss2_hit
        lda <boss_state
        cmpa #1
        lbne boss2_hit_done
        tst <boss_phase
        lbne boss2_hit_done
        ldy #ENTITIES+7*32
boss2_hit_arrow
        tst ,y
        beq boss2_hit_next
        lda 1,y
        lsla
        ldu #ai_boss_weapon_boxes
        leau a,u
        lda ,u
        sta <ai_boss_adjust
        adda #16
        sta <ai_boss_width
        lda 2,y
        suba <boss_x
        adda <ai_boss_adjust
        cmpa <ai_boss_width
        bhs boss2_hit_next
        lda 4,y
        suba <boss_y
        cmpa #16
        bhs boss2_hit_next
        clr ,y
        jsr sfx_hit
        lda <boss_hp
        suba 1,u
        bls boss2_killed
        sta <boss_hp
        bra boss2_hit_next
boss2_killed
        clr <boss_hp
        lda <boss_y
        suba #16
        sta <boss_y
        jsr boss_begin_death
        ldd #10000
        jmp ai_add_score
boss2_hit_next
        leay 32,y
        cmpy #ENTITIES+10*32
        blo boss2_hit_arrow
boss2_hit_done
        jmp boss2_hit_drops

boss2_draw
        lda <boss_state
        lbeq boss2_draw_done
        cmpa #3
        lbeq boss2_draw_done
        cmpa #2
        bne boss2_draw_visible
        lda <boss_wait
        bita #2
        lbne boss2_draw_done
boss2_draw_visible
        lda <boss_y
        cmpa #14
        lblo boss2_draw_done
        cmpa #145
        lbhs boss2_draw_done
        lda <boss_x
        cmpa #11
        lblo boss2_draw_done
        cmpa #228
        lbhs boss2_draw_done
        lda #BOSS2_ART
        ldb #8
        mul
        ldx #all_art
        leax d,x
        lda #BOSS2_ART
        ldu #all_art_banks
        lda a,u
        sta $ffa6
        lda <boss_x
        suba #11
        sta <art_x
        anda #1
        beq boss2_even_body
        ldd 2,x
        bra boss2_body_pointer
boss2_even_body
        ldd ,x
boss2_body_pointer
        std <boss_source
        lda <art_x
        lsra
        sta <span_x
        sta <boss_old_x
        clra
        ldb <ring_head
        addb <boss_y
        adca #0
        subd #14
        std <span_row
        std <boss_old_row
        lda <boss_hp
        lsla
        clrb
boss2_scale_hp
        cmpa #3
        blo boss2_hp_scaled
        suba #3
        incb
        bra boss2_scale_hp
boss2_hp_scaled
        stb B2_BAR
        lda #15
        ldb <boss_hp
        cmpb #20
        bhi boss2_color_ready
        lda #10
        cmpb #10
        bhi boss2_color_ready
        lda #7
boss2_color_ready
        sta B2_COLOR
        lsla
        lsla
        lsla
        lsla
        sta B2_COLOR_HI
        lda #20
        sta <span_n
        lda #46
        sta <art_lines
        ldy #$2300
        jsr framebuffer_pointer
boss2_body_row
        lda <art_lines
        cmpa #42
        bls boss2_body_pixels
        ldx #SCANLINE
        ldb #20
boss2_bar_pixel
        lda #$11
        cmpb B2_BAR
        bhi boss2_bar_empty
        lda #$ff
boss2_bar_empty
        sta ,x+
        decb
        bne boss2_bar_pixel
        ldx #SCANLINE
        jsr boss2_opaque_bar
        bra boss2_body_row_done
boss2_body_pixels
        ldx <boss_source
        jsr raw_boss_row
        stx <boss_source
boss2_body_row_done
        jsr advance_span
        dec <art_lines
        bne boss2_body_row
        lda #2
        sta <boss_drawn
boss2_draw_done
        rts
boss2_opaque_bar
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        ldd ,u
        std ,y++
        ldd ,x++
        std ,u++
        rts

B2_COLOR_HI equ $0dc4
B2_BAR equ $0dc5
boss2_hit_drops
        ldx #ENTITIES+10*32
boss2_hit_drop_loop
        tst ,x
        beq boss2_hit_drop_next
        tst 28,x
        beq boss2_hit_drop_next
        lda 2,x
        sta <rect_bx
        lda 4,x
        sta <rect_by
        lda #10
        sta <rect_bw
        sta <rect_bh
        ldy #ENTITIES+7*32
boss2_drop_arrow
        tst ,y
        beq boss2_drop_arrow_next
        jsr player_shot_rect
        jsr rect_hit
        tsta
        beq boss2_drop_arrow_next
        clr ,x
        clr ,y
        jsr sfx_hit
        bra boss2_hit_drop_next
boss2_drop_arrow_next
        leay 32,y
        cmpy #ENTITIES+10*32
        blo boss2_drop_arrow
boss2_hit_drop_next
        leax 32,x
        cmpx #ENTITIES+15*32
        blo boss2_hit_drop_loop
        rts
