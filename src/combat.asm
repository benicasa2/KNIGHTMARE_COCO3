; Native first-stage combat, ROM-derived enemy/boss AI and segment checkpoints.
; Logic/evidence: analysis/IA-E-CHECKPOINTS.md. Renderer/audio adaptations remain.
ENTITIES equ $2700
ENTITY_END equ $2960
; Each of 18 slots: active,kind,x8.8,y8.8,vx8.8,vy8.8,hp,age,state,cool,art,...
; 0..6 enemies, 7..9 player arrows, 10..17 hostile shots.
; +17 drawn flag, +18 old physical row, +20 xbyte, +21 width, +22 height.

combat_init
        ldd <high_score
        pshs d
        ldx #ENTITIES
combat_clear_slots
        clr ,x+
        cmpx #ENTITY_END
        blo combat_clear_slots
        ldx #combat_enabled
combat_clear_state
        clr ,x+
        cmpx #combat_frames+2
        blo combat_clear_state
        puls d
        std <high_score
        lda #1
        sta <combat_enabled
        lda #3
        sta <player_lives
        clr <player_invul
        lda #1
        sta <hud_dirty
        clr <respawn_pending
        clr <test_invincible
        clr <checkpoint_segment
        clr <satellite_clocks
        clr <satellite_clocks+1
        clr <satellite_clocks+2
        jsr boss_save_satellite_fractions
        ldd #ENTITIES+7*32
        std <enemy_limit_end
        jsr ai_init
        rts

; Erase in the reverse order of drawing, preserving overlap backgrounds.
combat_erase
        lda #18
        sta <entity_index
        ldx #ENTITIES+18*32
combat_erase_next
        stx <entity_ptr
        tst 17,x
        beq combat_erase_skip
        ldd 18,x
        std <span_row
        lda 20,x
        sta <span_x
        lda 21,x
        sta <span_n
        lda 22,x
        sta <art_lines
        ldx 30,x
        jsr restore_span_select
        jsr framebuffer_pointer
combat_erase_line
        jsr [blit_fn]
        jsr advance_span
        dec <art_lines
        bne combat_erase_line
        ldx <entity_ptr
        clr 17,x
combat_erase_skip
        leax -32,x
        dec <entity_index
        bpl combat_erase_next
        jsr erase_boss
        rts

combat_tick
        ldd <combat_frames
        addd #1
        std <combat_frames
        lda <game_ticks+1
        lsla
        deca
        sta <ai_clock
        tst <player_invul
        beq combat_no_invul
        dec <player_invul
combat_no_invul
        tst <combat_status
        lbeq combat_alive
        lda <combat_status
        cmpa #3
        lbeq cutscene_tick
        cmpa #1
        bne combat_tick_return
        dec <death_clock
        bne combat_death_updates
        tst <player_lives
        bne combat_respawn
        lda #2
        sta <respawn_pending
        rts
combat_respawn
        lda #1
        sta <respawn_pending
        rts
combat_death_updates
        tst <boss_state
        beq combat_death_regular
        jsr update_boss
        bra combat_death_enemies
combat_death_regular
        jsr spawn_waves
combat_death_enemies
        jsr move_enemies
combat_tick_return
        rts
combat_alive
        jsr fire_player
        jsr move_projectiles
        jsr items_tick
        ldd <freeze_ticks
        bne combat_collisions
        tst <boss_state
        bne combat_boss_active
        ldd <camera_y
        cmpd #120
        bhi combat_waves
        jsr start_boss
        bra combat_boss_active
combat_waves
        jsr spawn_waves
        jsr move_enemies
        bra combat_collisions
combat_boss_active
        jsr update_boss
        inc <ai_clock
        jsr move_enemies
        dec <ai_clock
combat_collisions
        jsr hit_enemies
        jsr hit_boss
        jsr touch_player
        rts

fire_player
        jmp items_fire_player

move_projectiles
        ldx #ENTITIES+7*32
move_projectile_next
        tst ,x
        beq projectile_skip
        cmpx #ENTITIES+10*32
        blo projectile_player_motion
        tst 28,x
        bne projectile_skip
        jsr ai_integrate
        bra projectile_skip
projectile_player_motion
        jsr weapon_projectile_tick
projectile_enemy_motion
        ldd 4,x
        addd 8,x
        std 4,x
        cmpa #168
        bhs projectile_kill
        ldd 2,x
        addd 6,x
        std 2,x
        cmpa #248
        bhs projectile_kill
        cmpx #ENTITIES+10*32
        blo projectile_skip
        cmpa #8
        blo projectile_kill
        bra projectile_skip
projectile_kill
        clr ,x
projectile_skip
        leax 32,x
        cmpx #ENTITIES+18*32
        blo move_projectile_next
        rts

        include "ai.asm"

; AABB helpers. Return A=1 on overlap; all rectangles stay inside 256x176.
rect_hit
        lda <rect_ax
        adda <rect_aw
        bcs rect_xa_ok
        cmpa <rect_bx
        bls rect_miss
rect_xa_ok
        lda <rect_bx
        adda <rect_bw
        bcs rect_xb_ok
        cmpa <rect_ax
        bls rect_miss
rect_xb_ok
        lda <rect_ay
        adda <rect_ah
        bcs rect_ya_ok
        cmpa <rect_by
        bls rect_miss
rect_ya_ok
        lda <rect_by
        adda <rect_bh
        bcs rect_yb_ok
        cmpa <rect_ay
        bls rect_miss
rect_yb_ok
        lda #1
        rts
rect_miss
        clra
        rts
player_rect
        lda <player_x
        adda #3
        sta <rect_ax
        lda <player_y
        adda #3
        sta <rect_ay
        lda #10
        sta <rect_aw
        sta <rect_ah
        rts
enemy_rect
        lda 1,x
        anda #15
        lsla
        lsla
        ldu #original_enemy_boxes
        leau a,u
        lda ,u+
        adda 4,x
        sta <rect_by
        lda ,u+
        adda 2,x
        sta <rect_bx
        lda ,u+
        sta <rect_bh
        lda ,u
        sta <rect_bw
        rts
hit_enemies
        ldx #ENTITIES
hit_enemy_next
        stx <entity_ptr
        lda ,x
        cmpa #1
        lbne hit_enemy_skip
        lda 1,x
        cmpa #7
        bne hit_enemy_not_seven
        lda 12,x
        cmpa #4
        lbhs hit_enemy_skip
hit_enemy_not_seven
        lda 1,x
        bne hit_enemy_not_zero
        lda 12,x
        cmpa #3
        lblo hit_enemy_skip
        cmpa #5
        lbhs hit_enemy_skip
hit_enemy_not_zero
        lda 1,x
        cmpa #12
        bne hit_enemy_phase_ready
        tst <boss_state
        bne hit_enemy_phase_ready
        lda 12,x
        cmpa #3
        lbhs hit_enemy_skip
hit_enemy_phase_ready
        lda 1,x
        cmpa #16
        lbhs hit_enemy_skip
        jsr enemy_rect
        ldy #ENTITIES+7*32
hit_enemy_arrow
        tst ,y
        beq hit_arrow_next
        jsr player_shot_rect
        jsr rect_hit
        tsta
        beq hit_arrow_next
        lda 1,x
        cmpa #7
        beq hit_enemy_seven_hit
        cmpa #12
        bne hit_enemy_normal_hit
        tst <boss_state
        bne hit_enemy_normal_hit
        lda #3
        sta 12,x
        lda 1,y
        cmpa #6
        bhs hit_enemy_skip
        clr ,y
        bra hit_enemy_skip
hit_enemy_seven_hit
        lda 1,y
        cmpa #6
        bhs hit_enemy_seven_keep_shot
        clr ,y
hit_enemy_seven_keep_shot
        dec 10,x
        beq hit_enemy_power_weapon
        lda #4
        sta 12,x
        jsr sfx_hit
        bra hit_enemy_skip
hit_enemy_normal_hit
        lda 1,y
        cmpa #6
        bhs hit_enemy_power_weapon
        clr ,y
        ldx <entity_ptr
        jsr sfx_hit
        dec 10,x
        lbne hit_enemy_skip
hit_enemy_power_weapon
        ldx <entity_ptr
        clr 10,x
        jsr kill_enemy
        bra hit_enemy_skip
hit_arrow_next
        leay 32,y
        cmpy #ENTITIES+10*32
        blo hit_enemy_arrow
hit_enemy_skip
        ldx <entity_ptr
        leax 32,x
        cmpx #ENTITIES+7*32
        lblo hit_enemy_next
        rts
kill_enemy
        jsr sfx_explosion
        lda #2
        sta ,x
        lda #18
        sta 11,x
        lda #EXPLOSION_ART
        sta 14,x
        lda 1,x
        anda #15
        lsla
        ldu #combat_points
        ldd a,u
        addd <score
        bcc score_store
        ldd #65535
score_store
        std <score
        cmpd <high_score
        bls no_high_score
        std <high_score
no_high_score
        ldd <kills
        addd #1
        std <kills
        lda #1
        sta <hud_dirty
        rts
touch_player
        tst <test_invincible
        lbne touch_return
        jsr shield_intercepts
        lda <ability_kind
        cmpa #2
        lbeq touch_return
        tst <player_invul
        lbne touch_return
        tst <combat_status
        lbne touch_return
        jsr player_rect
        ldx #ENTITIES
touch_enemy_loop
        lda 1,x
        cmpa #7
        bne touch_enemy_state_ready
        lda 12,x
        cmpa #4
        bhs touch_enemy_skip
touch_enemy_state_ready
        lda 1,x
        cmpa #16
        bhs touch_enemy_skip
        lda ,x
        cmpa #1
        bne touch_enemy_skip
        jsr enemy_rect
        jsr rect_hit
        tsta
        beq touch_enemy_skip
        lda <ability_kind
        cmpa #3
        beq touch_invincible_enemy
        lda #2
        sta ,x
        lda #18
        sta 11,x
        jmp player_hit
touch_invincible_enemy
        jsr kill_enemy
touch_enemy_skip
        leax 32,x
        cmpx #ENTITIES+7*32
        blo touch_enemy_loop
        ldx #ENTITIES+10*32
touch_bullet_loop
        tst ,x
        beq touch_bullet_skip
        lda 2,x
        sta <rect_bx
        lda 4,x
        sta <rect_by
        lda 1,x
        deca
        lsla
        ldu #ai_enemy_shot_boxes
        leau a,u
        ldd ,u
        std <rect_bw
        jsr rect_hit
        tsta
        beq touch_bullet_skip
        clr ,x
        lda <ability_kind
        cmpa #3
        lbne player_hit
touch_bullet_skip
        leax 32,x
        cmpx #ENTITIES+18*32
        blo touch_bullet_loop
        lda <ability_kind
        cmpa #3
        beq touch_return
        lda <boss_state
        cmpa #1
        lbne touch_return
        tst <campaign_stage
        beq touch_first_boss
        lda <boss_x
        suba #5
        sta <rect_bx
        lda <boss_y
        suba #4
        sta <rect_by
        lda #32
        sta <rect_bw
        lda #26
        sta <rect_bh
        bra touch_boss_box_ready
touch_first_boss
        lda <boss_x
        adda #2
        sta <rect_bx
        lda <boss_y
        adda #2
        sta <rect_by
        lda #36
        sta <rect_bw
        sta <rect_bh
touch_boss_box_ready
        jsr rect_hit
        tsta
        beq touch_return
player_hit
        tst <test_invincible
        bne touch_return
        jsr sfx_player
        lda #1
        sta <combat_status
        lda #48
        sta <death_clock
        dec <player_lives
        lda #1
        sta <hud_dirty
touch_return
        rts

; Original terrain predicate $620c: $43-$7f and $a3-$df are solid.
terrain_check
        lda <player_fx
        sta <old_player_fx
        lda <player_fy
        sta <old_player_fy
        lda <player_x
        sta <old_player_x
        lda <player_y
        sta <old_player_y
        rts
terrain_after_move
        lda <player_x
        adda #4
        sta <terrain_x
        lda <player_y
        adda #13
        sta <terrain_y
        jsr terrain_tile
        tsta
        beq terrain_block
        lda <player_x
        adda #11
        sta <terrain_x
        jsr terrain_tile
        tsta
        bne terrain_return
terrain_block
        lda <old_player_x
        sta <player_x
        lda <old_player_y
        sta <player_y
        lda <old_player_fx
        sta <player_fx
        lda <old_player_fy
        sta <player_fy
terrain_return
        rts
terrain_tile
        clra
        ldb <terrain_y
        addd <camera_y
        jsr campaign_map_row
        lda <terrain_x
        lsra
        lsra
        lsra
        lda a,x
        jsr items_terrain
        ; MSX $620C returns carry for SOLID tiles, not for walkable ones.
        ; Starting grass $80..$82 must remain traversable.
        suba #$43
        cmpa #$3d
        blo terrain_solid
        suba #$60
        cmpa #$3d
        blo terrain_solid
        ldb #1
        bra terrain_tile_restore
terrain_solid
        clrb
terrain_tile_restore
        lda #$0d
        sta $ffa6
        tfr b,a
        rts

        include "boss_ai.asm"
        include "checkpoints.asm"

; Generic masked sprites, clipped as complete images, with per-slot backgrounds.
combat_draw
        jsr draw_boss
        clr <entity_index
        ldx #ENTITIES
combat_draw_next
        stx <entity_ptr
        tst ,x
        lbeq combat_draw_skip
        lda 14,x
        cmpa #255
        lbeq combat_draw_skip
        ldu #all_art_banks
        lda a,u
        sta $ffa6
        lda 14,x
        ldb #8
        mul
        addd #all_art
        tfr d,u
        stu <art_ptr
        lda 6,u
        bpl combat_positive_x
        adda 2,x
        lbcc combat_draw_skip
        bra combat_x_ready
combat_positive_x
        adda 2,x
        lbcs combat_draw_skip
combat_x_ready
        sta <art_x
        lda 7,u
        bpl combat_positive_y
        adda 4,x
        lbcc combat_draw_skip
        bra combat_y_ready
combat_positive_y
        adda 4,x
        lbcs combat_draw_skip
combat_y_ready
        sta <art_y
        adda 5,u
        lbcs combat_draw_skip
        cmpa #176
        lbhi combat_draw_skip
        lda 4,u
        sta <art_width
        lsla
        adda <art_x
        lbcs combat_draw_skip
        lda 5,u
        sta <art_height
        sta <art_lines
        ldd ,u
        lda <art_x
        bita #1
        beq combat_even_art
        ldd 2,u
        bra combat_art_selected
combat_even_art
        ldd ,u
combat_art_selected
        std <art_source
        lda <art_x
        lsra
        sta <span_x
        sta 20,x
        lda <art_width
        sta <span_n
        sta 21,x
        ldu #raw_row_table
        lsla
        ldd a,u
        std <blit_fn
        lda <entity_index
        cmpa #18
        bne combat_regular_mask
        tst <token_family
        beq combat_regular_mask
        ldd #raw_token_row
        std <blit_fn
combat_regular_mask
        lda <art_height
        sta 22,x
        clra
        ldb <ring_head
        addb <art_y
        adca #0
        std <span_row
        std 18,x
        lda #1
        sta 17,x
        lda <entity_index
        cmpa #18
        bne combat_draw_standard_buffer
        ldy #$0f00
        bra combat_draw_buffer_ready
combat_draw_standard_buffer
        ldb 1,x
        cmpb #7
        bne combat_draw_normal_bg
        lda 28,x
        anda #1
        lsla
        adda #$2a
        clrb
        tfr d,y
        bra combat_draw_buffer_ready
combat_draw_normal_bg
        adda #$11
        clrb
        tfr d,y
combat_draw_buffer_ready
        sty 30,x
        jsr framebuffer_pointer
combat_draw_row
        ldx <art_source
        jsr [blit_fn]
        stx <art_source
        jsr advance_span
        dec <art_lines
        bne combat_draw_row
combat_draw_skip
        ldx <entity_ptr
        leax 32,x
        inc <entity_index
        lda <entity_index
        cmpa #19
        lblo combat_draw_next
        lda #$0d
        sta $ffa6
        rts

erase_boss
        tst <boss_drawn
        beq erase_boss_return
        ldd <boss_old_row
        std <span_row
        lda <boss_old_x
        sta <span_x
        lda #20
        sta <span_n
        lda #44
        tst <campaign_stage
        beq erase_boss_height_ready
        lda #46
erase_boss_height_ready
        sta <art_lines
        ldx #$2300
        jsr restore_span_select
        jsr framebuffer_pointer
erase_boss_line
        jsr [blit_fn]
        jsr advance_span
        dec <art_lines
        bne erase_boss_line
        clr <boss_drawn
erase_boss_return
        rts
draw_boss
        tst <campaign_stage
        lbne boss2_draw
        lda <boss_state
        lbeq draw_boss_return
        cmpa #3
        lbeq draw_boss_return
        cmpa #2
        bne boss_draw_normal
        lda <boss_wait
        bita #2
        lbne draw_boss_return
boss_draw_normal
        lda <boss_y
        cmpa #4
        lblo draw_boss_return
        cmpa #133
        lbhs draw_boss_return
        lda #14
        sta $ffa6
        lda <boss_clock+1
        anda #16
        lsra
        lsra
        lsra
        ldx #boss_pointers
        ldx a,x
        stx <boss_source
        clra
        ldb <ring_head
        addb <boss_y
        adca #0
        subd #4
        std <span_row
        std <boss_old_row
        lda <boss_x
        lsra
        sta <span_x
        sta <boss_old_x
        lda #20
        sta <span_n
        lda #44
        sta <art_lines
        ldy #$2300
        jsr framebuffer_pointer
boss_draw_line
        lda <art_lines
        cmpa #40
        bls boss_copy_art
        ldx #SCANLINE
        ldb #20
boss_bar_line
        lda #$11
        cmpb <boss_hp
        bhi boss_bar_empty
        lda #$ff
boss_bar_empty
        sta ,x+
        decb
        bne boss_bar_line
        ldx #SCANLINE
        bra boss_write_line
boss_copy_art
        ldx <boss_source
        leax 20,x
        stx <boss_source
        leax -20,x
boss_write_line
        jsr boss2_opaque_bar
        jsr advance_span
        dec <art_lines
        bne boss_draw_line
        lda #1
        sta <boss_drawn
draw_boss_return
        rts

hud_update
        tst <hud_dirty
        beq hud_update_return
        clr <hud_dirty
        lda #$0d
        sta $ffa6
        ldd <score
        jsr decimal_digits
        lda #2
        sta <hud_col
        jsr hud_six_digits
        ldd <high_score
        jsr decimal_digits
        lda #11
        sta <hud_col
        jsr hud_six_digits
        lda #29
        sta <hud_col
        clrb
        jsr hud_digit
        inc <hud_col
        ldb <campaign_stage
        incb
        jsr hud_digit
        lda #21
        sta <hud_col
        clrb
        jsr hud_digit
        inc <hud_col
        ldb <player_lives
        beq hud_rest_zero
        decb
hud_rest_zero
        jsr hud_digit
        jsr draw_hud
hud_update_return
        rts
decimal_digits
        std <aim_vx
        clr <hud_digits
        ldx #decimal_divisors
        ldy #hud_digits+1
        lda #5
        sta <digit_count
        ldd <aim_vx
decimal_digit_loop
        clr <digit_temp
decimal_subtract
        cmpd ,x
        blo decimal_digit_ready
        subd ,x
        inc <digit_temp
        bra decimal_subtract
decimal_digit_ready
        pshs d
        lda <digit_temp
        sta ,y+
        puls d
        leax 2,x
        dec <digit_count
        bne decimal_digit_loop
        rts
hud_six_digits
        ldy #hud_digits
        lda #6
        sta <digit_count
hud_digit_next
        ldb ,y+
        jsr hud_digit
        inc <hud_col
        dec <digit_count
        bne hud_digit_next
        rts
hud_digit
        pshs x,y
        addb #16
        lda #32
        mul
        addd #$a000
        tfr d,x
        lda <hud_col
        ldb #4
        mul
        addd #$d400
        tfr d,u
        lda #8
        sta <digit_temp
hud_font_row
        ldd ,x++
        std ,u++
        ldd ,x++
        std ,u++
        leau 124,u
        dec <digit_temp
        bne hud_font_row
        puls x,y,pc
decimal_divisors
        fdb 10000,1000,100,10,1
        include "combat_data.inc"
