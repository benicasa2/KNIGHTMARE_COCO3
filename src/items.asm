TOKEN_SLOT equ ENTITIES+18*32
FIXED_STATES equ $2980

items_init
        clr <scene_mode
        clr <shield_drawn
        clr <terrain_overlays
        clr <weapon_kind
        clr <speed_level
        clr <ability_kind
        clr <ability_timer
        clr <ability_timer+1
        clr <shield_hits
        clr <freeze_ticks
        clr <freeze_ticks+1
        clr <player_fx
        clr <player_fy
        clr <token_index
        clr <token_family
        clr <token_selection
        ldx #FIXED_STATES
        jsr phase_fixed_table
        tfr x,y
        ldx #FIXED_STATES
        ldb PHASE_FIXED_COUNT
items_init_fixed
        clr ,x
        lda 4,y
        sta 1,x
        lda 3,y
        anda #31
        cmpa #23
        blo items_init_not_portal
        lda #5
        sta ,x
        bra items_init_next
items_init_not_portal
        cmpa #21
        bne items_init_not_exit
        tst <unlocked_exit
        bne items_init_not_exit
        lda #6
        sta ,x
        bra items_init_next
items_init_not_exit
        lda 3,y
        bita #32
        beq items_init_next
        lda #1
        sta ,x
items_init_next
        leax 2,x
        leay 6,y
        decb
        bne items_init_fixed
        rts

items_tick
        ldd <ability_timer
        beq items_no_ability_time
        subd #1
        std <ability_timer
        bne items_no_ability_time
        clr <ability_kind
        tst <shield_hits
        beq items_no_ability_time
        inc <ability_kind
items_no_ability_time
        ldd <freeze_ticks
        beq items_no_freeze_time
        subd #1
        std <freeze_ticks
items_no_freeze_time
        jsr floating_token_tick
        jsr fixed_items_tick
        rts

; The original list is positional. No invented random item drops.
floating_token_tick
        tst <boss_state
        beq token_not_boss
        clr TOKEN_SLOT
        rts
token_not_boss
        ldx #TOKEN_SLOT
        tst ,x
        bne token_move
        lda <token_index
        cmpa PHASE_TOKEN_COUNT
        bhs token_return
        ldb #3
        mul
        pshs x
        jsr phase_token_table
        tfr x,u
        puls x
        leau d,u
        ldd <camera_y
        cmpd ,u
        bhi token_return
        inc <token_index
        cmpd ,u
        blo token_return
        lda #1
        sta ,x
        lda 2,u
        sta <token_family
        clr <token_selection
        clr <token_angle
        lda <player_x
        cmpa #40
        bhs token_center_low
        lda #40
token_center_low
        cmpa #208
        bls token_center_ready
        lda #208
token_center_ready
        sta <token_center
        ldd #$e000
        std 4,x
token_move
        ldd 4,x
        ; $583D runs the floating-object update on alternate MSX frames:
        ; its raw $009F step already matches our approximately 30 Hz tick.
        addd #159
        std 4,x
        cmpa #168
        blo token_inside
        cmpa #216
        bhs token_inside
        clr ,x
token_return
        rts
token_inside
        lda <token_angle
        adda #64
        sta <token_angle
        ldu #token_cosine+128
        eora #128
        lda a,u
        adda <token_center
        sta 2,x
        clr 3,x
        lda 4,x
        cmpa #160
        bhs token_select_art
        jsr player_rect
        lda 2,x
        sta <rect_bx
        lda 4,x
        sta <rect_by
        lda #16
        sta <rect_bw
        sta <rect_bh
        jsr rect_hit
        tsta
        lbne token_collect
        ldy #ENTITIES+7*32
token_arrow_loop
        tst ,y
        beq token_next_arrow
        jsr player_shot_rect
        jsr rect_hit
        tsta
        beq token_next_arrow
        clr ,y
        inc <token_selection
        lda <token_family
        beq token_eight_choices
        lda <token_selection
        cmpa #7
        blo token_cycle_done
        clr <token_selection
        bra token_cycle_done
token_eight_choices
        lda <token_selection
        anda #7
        sta <token_selection
token_cycle_done
        jsr sfx_cycle
        bra token_select_art
token_next_arrow
        leay 32,y
        cmpy #ENTITIES+10*32
        blo token_arrow_loop
token_select_art
        ldx #TOKEN_SLOT
        lda <token_selection
        tst <token_family
        bne token_power_picture
        ldu #weapon_reward_table
        lda a,u
        ldb <weapon_kind
        andb #$fe
        stb <token_byte
        cmpa <token_byte
        bne token_weapon_picture
        inca
token_weapon_picture
        ldu #token_weapon_ids
        lda a,u
        sta 14,x
        rts
token_power_picture
        ldu #token_power_ids
        lda a,u
        sta 14,x
        lda <game_ticks+1
        lsra
        anda #3
        ldu #token_flash_colors
        lda a,u
        sta <token_ink
        lsla
        lsla
        lsla
        lsla
        sta <token_ink_hi
        rts
token_collect
        clr TOKEN_SLOT
        lda <token_selection
        tst <token_family
        bne token_gain_power
        ldu #weapon_reward_table
        ldb a,u
        beq token_blank
        lda <weapon_kind
        anda #$fe
        sta <token_byte
        cmpb <token_byte
        bne token_gain_weapon
        incb
token_gain_weapon
        stb <weapon_kind
        bra token_good_reward
token_gain_power
        ldu #power_reward_table
        ldb a,u
        beq token_blank
        decb
        bne token_not_speed
        lda <speed_level
        cmpa #4
        bhs token_good_reward
        inc <speed_level
        bra token_good_reward
token_not_speed
        stb <ability_kind
        cmpb #1
        bne token_timed_power
        lda #30
        sta <shield_hits
        clr <ability_timer
        clr <ability_timer+1
        bra token_good_reward
token_timed_power
        ldd #450
        std <ability_timer
token_good_reward
        ldd #200
        bra token_score
token_blank
        ldd #1000
token_score
        jsr item_add_score
        jsr sfx_pickup
        rts

item_add_score
        addd <score
        bcc item_score_no_wrap
        ldd #65535
item_score_no_wrap
        std <score
        cmpd <high_score
        bls item_score_no_record
        std <high_score
item_score_no_record
        lda #1
        sta <hud_dirty
        rts

; Color index 14 is a marker for the star's original four-color flash.
token_masked_row
        lda <art_width
        sta <token_pixel
token_masked_byte
        lda ,u
        sta ,y+
        anda ,x+
        pshs a
        ldb ,x+
        stb <token_byte
        andb #$f0
        cmpb #$e0
        bne token_color_low
        ldb <token_byte
        andb #15
        orb <token_ink_hi
        stb <token_byte
token_color_low
        ldb <token_byte
        andb #15
        cmpb #14
        bne token_color_ready
        ldb <token_byte
        andb #$f0
        orb <token_ink
        stb <token_byte
token_color_ready
        lda <token_byte
        ora ,s+
        sta ,u+
        dec <token_pixel
        bne token_masked_byte
        rts

; Fixed objects remain world-space tiles, as in the MSX; the token uses one
; additional masked-sprite slot. Map ROM data itself is never mutated.
fixed_begin
        jsr phase_fixed_table
        stx <fixed_ptr
        ldu #FIXED_STATES
        stu <fixed_state_ptr
        clr <fixed_index
        rts
fixed_next
        ldx <fixed_ptr
        leax 6,x
        stx <fixed_ptr
        ldu <fixed_state_ptr
        leau 2,u
        stu <fixed_state_ptr
        inc <fixed_index
        lda <fixed_index
        cmpa PHASE_FIXED_COUNT
        rts

; Called after TILE_ROW has been copied, before expanding its 32 tiles.
items_overlay_line
        jsr fixed_begin
fixed_overlay_loop
        lda ,u
        beq fixed_overlay_next
        cmpa #5
        bhs fixed_overlay_next
        ldd <world_line
        subd ,x
        lda ,u
        cmpa #4
        bne fixed_regular_row
        ; Recompute D: loading the state above replaced the high byte.
        ldd <world_line
        subd ,x
        addd #16
        tsta
        bne fixed_overlay_next
        cmpb #32
        bhs fixed_overlay_next
        lsrb
        lsrb
        lsrb
        lslb
        lslb
        ldu #bridge_item_tiles
        leau b,u
        lda 2,x
        lsra
        lsra
        lsra
        deca
        sta <fixed_col
        ldb #4
        bra fixed_copy_tiles
fixed_regular_row
        ldd <world_line
        subd ,x
        tsta
        bne fixed_overlay_next
        cmpb #16
        bhs fixed_overlay_next
        andb #8
        lsrb
        lsrb
        stb <fixed_rel
        ldu <fixed_state_ptr
        lda ,u
        cmpa #1
        beq fixed_question_tiles
        cmpa #3
        beq fixed_spent_tiles
        lda 3,x
        anda #7
        bra fixed_tiles_kind
fixed_question_tiles
        lda #7
        bra fixed_tiles_kind
fixed_spent_tiles
        lda #6
fixed_tiles_kind
        lsla
        lsla
        adda <fixed_rel
        ldu #fixed_item_tiles
        leau a,u
        lda 2,x
        lsra
        lsra
        lsra
        sta <fixed_col
        ldb #2
fixed_copy_tiles
        ldy #TILE_ROW
        lda <fixed_col
        leay a,y
fixed_copy_tile
        lda ,u+
        sta ,y+
        decb
        bne fixed_copy_tile
fixed_overlay_next
        jsr fixed_next
        lblo fixed_overlay_loop
        rts

fixed_items_tick
        jsr fixed_begin
fixed_tick_loop
        lda ,u
        cmpa #6
        lbeq fixed_tick_next
        ldd ,x
        subd <camera_y
        tsta
        lbne fixed_tick_next
        cmpb #176
        lbhs fixed_tick_next
        stb <fixed_screen_y
        lda ,u
        cmpa #5
        lbeq fixed_portal
        cmpa #4
        lbeq fixed_tick_next
        cmpa #3
        lbeq fixed_tick_next
        cmpa #2
        lbeq fixed_reward_touch
        ; Hidden/visible question: each projectile consumes one original HP.
        lda 2,x
        sta <rect_bx
        lda <fixed_screen_y
        sta <rect_by
        lda #16
        sta <rect_bw
        sta <rect_bh
        ldy #ENTITIES+7*32
fixed_arrow_loop
        tst ,y
        beq fixed_arrow_next
        jsr player_shot_rect
        jsr rect_hit
        tsta
        beq fixed_arrow_next
        clr ,y
        ldu <fixed_state_ptr
        clr <fixed_changed
        tst ,u
        bne fixed_already_revealed
        lda #1
        sta ,u
        inc <fixed_changed
fixed_already_revealed
        dec 1,u
        bne fixed_object_hit
        lda #2
        sta ,u
        lda 3,x
        anda #7
        cmpa #6
        bne fixed_object_broken
        lda #4
        sta ,u
fixed_object_broken
        lda 3,x
        anda #7
        cmpa #4
        beq fixed_has_terrain
        cmpa #6
        bne fixed_no_terrain
fixed_has_terrain
        inc <terrain_overlays
fixed_no_terrain
        inc <fixed_changed
fixed_object_hit
        jsr sfx_cycle
        tst <fixed_changed
        beq fixed_arrow_next
        jsr fixed_refresh
fixed_arrow_next
        leay 32,y
        cmpy #ENTITIES+10*32
        blo fixed_arrow_loop
        bra fixed_tick_next
fixed_reward_touch
        lda 3,x
        anda #7
        cmpa #4
        beq fixed_tick_next
        jsr player_rect
        lda 2,x
        sta <rect_bx
        lda <fixed_screen_y
        sta <rect_by
        lda #16
        sta <rect_bw
        sta <rect_bh
        jsr rect_hit
        tsta
        beq fixed_tick_next
        lda #3
        sta ,u
        lda 3,x
        anda #7
        jsr fixed_apply_reward
        jsr fixed_refresh
        bra fixed_tick_next
fixed_portal
        lda 4,x
        anda #$f0
        lsla
        sta <rect_bh
        jsr player_rect
        lda 2,x
        sta <rect_bx
        lda <fixed_screen_y
        sta <rect_by
        lda #8
        sta <rect_bw
        jsr rect_hit
        tsta
        beq fixed_tick_next
        lda <input_bits
        ldb 4,x
        bitb #1
        bne fixed_portal_right
        bita #4
        beq fixed_tick_next
        bra fixed_portal_warp
fixed_portal_right
        bita #8
        beq fixed_tick_next
fixed_portal_warp
        lda #240
        suba <player_x
        cmpa #239
        bls fixed_portal_destination
        lda #239
fixed_portal_destination
        sta <player_x
fixed_tick_next
        jsr fixed_next
        lblo fixed_tick_loop
        rts

fixed_apply_reward
        cmpa #0
        bne fixed_not_life
        lda <player_lives
        cmpa #99
        bhs fixed_reward_sound
        inc <player_lives
        lda #1
        sta <hud_dirty
        bra fixed_reward_sound
fixed_not_life
        cmpa #1
        bne fixed_not_points
        ldd #500
        jsr item_add_score
        bra fixed_reward_sound
fixed_not_points
        cmpa #2
        bne fixed_not_bomb
        pshs x,u
        ldx #ENTITIES
fixed_bomb_loop
        lda ,x
        cmpa #1
        bne fixed_bomb_next
        jsr kill_enemy
fixed_bomb_next
        leax 32,x
        cmpx #ENTITIES+7*32
        blo fixed_bomb_loop
        puls x,u
        bra fixed_clear_bullets
fixed_not_bomb
        cmpa #3
        bne fixed_not_freeze
        ; Original $1000 BCD clock decrements every VBL during freeze:
        ; ten groups of sixty frames => 600 original frames / 2.
        ldd #300
        std <freeze_ticks
fixed_clear_bullets
        pshs x
        ldx #ENTITIES+10*32
fixed_clear_bullet_loop
        clr ,x
        leax 32,x
        cmpx #ENTITIES+18*32
        blo fixed_clear_bullet_loop
        puls x
        bra fixed_reward_sound
fixed_not_freeze
        cmpa #5
        bne fixed_reward_sound
        ; Stage transitions remain limited to this first-stage build.
        lda #3
        sta <combat_status
        lda #$12
        sta $ff9a
fixed_reward_sound
        jsr sfx_pickup
        rts

; Only the modified object's 32 raster lines are rebuilt. Caller objects have
; already been erased, so no saved sprite background can be invalidated.
; At most one refresh per changed fixed object per native update.
; The queue occupies otherwise unused scratch RAM, not graphics banks.
FIXED_REFRESH_QUEUE equ $0d40
fixed_refresh
        tst <render_deferred
        beq fixed_refresh_now
        pshs d,x
        tst <render_refresh_pending
        bne fixed_queue_ready
        ldx #FIXED_REFRESH_QUEUE
        ldb PHASE_FIXED_COUNT
fixed_queue_clear
        clr ,x+
        decb
        bne fixed_queue_clear
        inc <render_refresh_pending
fixed_queue_ready
        ldx #FIXED_REFRESH_QUEUE
        lda <fixed_index
        ldb #1
        stb a,x
        puls d,x,pc

fixed_flush_refresh
        jsr fixed_begin
fixed_flush_next
        ldy #FIXED_REFRESH_QUEUE
        ldb <fixed_index
        tst b,y
        beq fixed_flush_skip
        jsr fixed_refresh_now
fixed_flush_skip
        jsr fixed_next
        blo fixed_flush_next
        clr <render_refresh_pending
        rts

fixed_refresh_now
        pshs d,x,y,u
        ldx <fixed_ptr
        ldd ,x
        subd #16
        std <refresh_world
        lda #32
        sta <refresh_count
fixed_refresh_row
        ldd <refresh_world
        subd <camera_y
        tsta
        bne fixed_refresh_skip
        cmpb #176
        bhs fixed_refresh_skip
        addb <ring_head
        adca #0
        ; Resolve the full physical row before converting to the 192-line ring.
        ; Rendering the canonical row refreshes both background copies.
        cmpd #192
        blo fixed_refresh_target_ready
        subd #192
fixed_refresh_target_ready
        stb <target_line
        ldd <refresh_world
        std <world_line
        ; Overlay uses the same iterator; preserve it around the renderer.
        ldx <fixed_ptr
        ldy <fixed_state_ptr
        lda <fixed_index
        pshs a,x,y
        jsr render_world_line
        puls a,x,y
        sta <fixed_index
        stx <fixed_ptr
        sty <fixed_state_ptr
fixed_refresh_skip
        ldd <refresh_world
        addd #1
        std <refresh_world
        dec <refresh_count
        bne fixed_refresh_row
        puls d,x,y,u,pc

; Densely packed original speed pairs, in 1/16 pixel per original frame.
item_movement_speed
        lda <speed_level
        ldb <ability_kind
        cmpb #3
        bne item_speed_level
        lda #4
item_speed_level
        lsla
        ldx #original_speed_pairs
        leax a,x
        clra
        ldb ,x
        lslb
        rola
        lslb
        rola
        lslb
        rola
        lslb
        rola
        lslb
        rola
        std <move_vy
        clra
        ldb 1,x
        lslb
        rola
        lslb
        rola
        lslb
        rola
        lslb
        rola
        lslb
        rola
        std <move_vx
        rts

; Weapon rectangles use original width/height, swept over one native step.
player_shot_rect
        pshs x,u
        lda 1,y
        anda #15
        lsla
        ldu #original_weapon_boxes
        leau a,u
        lda ,u
        adda #16
        sta <rect_ah
        lda 1,u
        sta <rect_aw
        lda 2,y
        sta <rect_ax
        lda 4,y
        sta <rect_ay
        puls x,u,pc

; Collision follows the same gray-block/bridge overlays as the renderer.
items_terrain
        tst <terrain_overlays
        bne items_terrain_needed
        rts
items_terrain_needed
        pshs b,x,y,u
        sta <terrain_original
        clra
        ldb <terrain_y
        addd <camera_y
        std <terrain_world
        jsr fixed_begin
items_terrain_loop
        lda ,u
        cmpa #4
        beq items_terrain_bridge
        cmpa #2
        bne items_terrain_next
        lda 3,x
        anda #7
        cmpa #4
        bne items_terrain_next
        ldd <terrain_world
        subd ,x
        tsta
        bne items_terrain_next
        cmpb #16
        bhs items_terrain_next
        lda 2,x
        anda #$f8
        sta <fixed_col
        lda <terrain_x
        suba <fixed_col
        cmpa #16
        bhs items_terrain_next
        lda #$4c
        sta <terrain_original
        bra items_terrain_done
items_terrain_bridge
        ldd <terrain_world
        subd ,x
        addd #16
        tsta
        bne items_terrain_next
        cmpb #32
        bhs items_terrain_next
        lda 2,x
        anda #$f8
        suba #8
        sta <fixed_col
        lda <terrain_x
        suba <fixed_col
        cmpa #32
        bhs items_terrain_next
        lda #$3f
        sta <terrain_original
        bra items_terrain_done
items_terrain_next
        jsr fixed_next
        blo items_terrain_loop
items_terrain_done
        lda <terrain_original
        puls b,x,y,u,pc

        include "items_data.inc"
