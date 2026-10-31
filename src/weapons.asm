items_fire_player
        tst <fire_clock
        beq weapon_fire_ready
        dec <fire_clock
weapon_fire_ready
        lda <ability_kind
        cmpa #3
        lbeq weapon_fire_return
        lda <input_bits
        bita #16
        lbeq weapon_fire_return
        tst <fire_clock
        lbne weapon_fire_return
        lda <player_y
        cmpa #8
        lblo weapon_fire_return
        lda <weapon_kind
        cmpa #8
        blo weapon_single_fire
        cmpa #10
        bhs weapon_single_fire
        ldx #ENTITIES+7*32
        tst ,x
        lbne weapon_fire_return
        tst 32,x
        lbne weapon_fire_return
        tst 64,x
        lbne weapon_fire_return
        clr <shot_member
weapon_triple_loop
        jsr weapon_initialize_shot
        inc <shot_member
        leax 32,x
        lda <shot_member
        cmpa #3
        blo weapon_triple_loop
        bra weapon_fired
weapon_single_fire
        ldu #weapon_slot_counts
        ldb a,u
        ldx #ENTITIES+7*32
weapon_find_free
        tst ,x
        beq weapon_free_found
        leax 32,x
        decb
        bne weapon_find_free
weapon_fire_return
        rts
weapon_free_found
        clr <shot_member
        jsr weapon_initialize_shot
weapon_fired
        lda #4
        sta <fire_clock
        jsr sfx_shot
        rts
weapon_initialize_shot
        lda #1
        sta ,x
        lda <weapon_kind
        sta 1,x
        ldu #weapon_art_ids
        ldb a,u
        stb 14,x
        ldu #weapon_x_offsets
        lda a,u
        adda <player_x
        clrb
        std 2,x
        lda <player_y
        suba #8
        std 4,x
        ldd #0
        std 6,x
        clr 11,x
        clr 12,x
        clr 16,x
        lda <shot_member
        sta 15,x
        lda <weapon_kind
        ldd #-$0800
        std 8,x
        lda <weapon_kind
        cmpa #2
        bne weapon_not_two
        ldd #-$0980
        std 8,x
weapon_not_two
        cmpa #4
        bne weapon_not_four
        ldd #-$0900
        std 8,x
weapon_not_four
        cmpa #8
        blo weapon_shot_count
        cmpa #10
        bhs weapon_shot_count
        lda <shot_member
        beq weapon_shot_count
        ldd #-$0759
        std 8,x
        ldd #-$0300
        lda <shot_member
        cmpa #1
        bne weapon_triple_right
        ldd #-$0300
        bra weapon_triple_velocity
weapon_triple_right
        ldd #$0300
weapon_triple_velocity
        std 6,x
weapon_shot_count
        ldd <shots_fired
        addd #1
        std <shots_fired
        rts

weapon_projectile_tick
        lda 1,x
        cmpa #6
        blo weapon_projectile_return
        cmpa #8
        bhs weapon_projectile_return
        ; A boomerang tracks the player's horizontal position and follows the
        ; original 22-step half-pixel ramp. $5859 also runs on alternate frames.
        lda <player_x
        sta 2,x
        clr 3,x
        lda 11,x
        ldu #original_boomerang_steps
        ldb a,u
        clra
        lsrb
        rora
        exg a,b
        ; D = original step * 128, signed below for outgoing travel.
        tst 12,x
        bne weapon_boomerang_back
        coma
        comb
        addd #1
        std 8,x
        lda 11,x
        inca
        cmpa #22
        blo weapon_store_ramp
        lda #1
        sta 12,x
        lda #21
        bra weapon_store_ramp
weapon_boomerang_back
        std 8,x
        lda 11,x
        suba #1
        bcc weapon_store_ramp
        clr ,x
        clra
weapon_store_ramp
        sta 11,x
        inc 16,x
        lda 16,x
        lsra
        lsra
        anda #3
        ldu #boomerang_art_ids
        lda a,u
        sta 14,x
weapon_projectile_return
        rts

shield_intercepts
        lda <ability_kind
        cmpa #1
        bne shield_return
        lda <player_x
        sta <rect_ax
        lda <player_y
        suba #9
        bcc shield_y_ready
        clra
shield_y_ready
        sta <rect_ay
        lda #16
        sta <rect_aw
        lda #8
        sta <rect_ah
        ldx #ENTITIES+10*32
shield_bullet_loop
        tst ,x
        beq shield_bullet_next
        lda 2,x
        suba #3
        sta <rect_bx
        lda 4,x
        suba #3
        sta <rect_by
        lda #6
        sta <rect_bw
        sta <rect_bh
        jsr rect_hit
        tsta
        beq shield_bullet_next
        clr ,x
        jsr sfx_hit
        ldb #1
        lda 1,x
        cmpa #7
        blo shield_charge_cost
        ldb #4
shield_charge_cost
        stb <token_byte
        lda <shield_hits
        suba <token_byte
        bhi shield_store_charge
        clr <shield_hits
        clr <ability_kind
        rts
shield_store_charge
        sta <shield_hits
shield_bullet_next
        leax 32,x
        cmpx #ENTITIES+18*32
        blo shield_bullet_loop
shield_return
        rts

; Original animation 1 adds nine shield rows above the unchanged walking body.
erase_shield
        tst <shield_drawn
        beq erase_shield_done
        ldd <shield_old_row
        std <span_row
        lda <shield_old_x
        sta <span_x
        lda #9
        sta <span_n
        sta <object_lines
        ldx #$0e90
        jsr restore_span_select
        jsr framebuffer_pointer
erase_shield_row
        jsr [blit_fn]
        jsr advance_span
        dec <object_lines
        bne erase_shield_row
        clr <shield_drawn
erase_shield_done
        rts
draw_shield
        lda <ability_kind
        cmpa #1
        bne draw_shield_done
        lda <player_y
        cmpa #9
        blo draw_shield_done
        suba #9
        tfr a,b
        clra
        addb <ring_head
        adca #0
        std <span_row
        std <shield_old_row
        lda <player_x
        lsra
        sta <span_x
        sta <shield_old_x
        ldx #shield_mask
        lda <player_x
        bita #1
        beq draw_shield_aligned
        leax 162,x
draw_shield_aligned
        stx <sprite_pointer
        ldy #$0e90
        lda #9
        sta <span_n
        sta <object_lines
        jsr framebuffer_pointer
draw_shield_row
        ldx <sprite_pointer
        ldb #9
draw_shield_byte
        lda ,u
        sta ,y+
        anda ,x+
        ora ,x+
        sta ,u+
        decb
        bne draw_shield_byte
        stx <sprite_pointer
        jsr advance_span
        dec <object_lines
        bne draw_shield_row
        lda #1
        sta <shield_drawn
draw_shield_done
        rts
