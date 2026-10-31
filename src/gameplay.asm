; Native 6809 controls and scrolling of the original stage tilemaps.
; RAM: 192-line ring + 192-line mirror, physical banks 0..5 (48 KiB).
; Bank 6 holds permanent auxiliary code/data; bank 7 holds PCM effects.
; CPU $8000 is a framebuffer window; $a000 tiles; $c000 sprites/HUD.
TILE_ROW        equ $0d00
SAVED_PLAYER    equ $0e00
SCANLINE        equ $1000
VIEW_HEIGHT     equ 176
MAP_HEIGHT      equ 1952

game_init
        jsr music_reset
        jsr sound_stop
        clr $ffb0
        clr $ff9a
        clr <campaign_stage
        jsr phase_profile
        jsr level_tiles_load
        tst FD_ERROR
        bne game_disk_error
        jsr combat_init
        jsr items_init
        jsr campaign_map_load
        ldd #MAP_HEIGHT-VIEW_HEIGHT
        std <camera_y
        bra game_rebuild
game_disk_error
        lda #4
        sta <scene_mode
        rts

game_rebuild
        lda #$0c
        sta $ffa5
        lda #$0d
        sta $ffa6
        clr <ring_head
        clr EXIT_STATE
        clr <ring_wraps
        clr <scroll_clock
        clr <paused
        clr <sprite_valid
        ldd #MAP_HEIGHT-VIEW_HEIGHT
        subd <camera_y
        std <scroll_steps
        lda #120
        sta <player_x
        lda #135
        sta <player_y
        clr <player_frame
        ldd <camera_y
        std <world_line
        clr <target_line
init_world_loop
        jsr render_world_line
        ldd <world_line
        addd #1
        std <world_line
        inc <target_line
        lda <target_line
        cmpa #VIEW_HEIGHT
        blo init_world_loop
        jsr hud_update
        jsr draw_hud
        jsr combat_draw
        jsr draw_player
        jsr publish_head
        rts

; One interactive update per two video IRQ ticks: 29.96 Hz, same movement.
game_tick
        inc <game_divider
        lda <game_divider
        bita #1
        lbne game_return
        ldd <game_ticks
        addd #1
        std <game_ticks
        jsr poll_input
apply_input
        lda <input_bits
        eora <previous_input
        anda <input_bits
        sta <input_edges
        lda <input_bits
        sta <previous_input
        lda <key_rows+5
        anda #2
        sta <music_key_edge
        eora <music_previous_key
        anda <music_key_edge
        ldb <music_key_edge
        stb <music_previous_key
        sta <music_key_edge
        tst <scene_mode
        beq game_scene_active
        jsr intro_tick
        jmp game_return
game_scene_active
        tst <music_key_edge
        beq no_music_toggle
        jsr music_toggle
no_music_toggle
        lda <input_edges
        bpl no_test_toggle
        tst <combat_status
        bne no_test_toggle
        lda <test_invincible
        eora #1
        sta <test_invincible
        beq test_border_black
        lda palette+4
        bra test_border_store
test_border_black
        clra
test_border_store
        sta $ff9a
no_test_toggle
        lda <input_edges
        bita #$20
        beq no_pause_key
        lda <paused
        eora #1
        sta <paused
no_pause_key
        lda <input_edges
        bita #$40
        beq no_reset_key
        jsr game_init
        rts
no_reset_key
        tst <paused
        lbne game_return
        tst <combat_enabled
        lbne game_combat_update
        jsr move_player
        clr <scroll_due
        inc <scroll_clock
        lda <scroll_clock
        cmpa #2
        blo scroll_timer_wait
        clr <scroll_clock
        ldd <camera_y
        beq scroll_timer_wait
        inc <scroll_due
scroll_timer_wait
        lda <scroll_due
        ora <player_moved
        beq game_return
        jsr erase_player
        tst <scroll_due
        beq no_camera_step
        dec <ring_head
        lda <ring_head
        cmpa #255
        bne ring_valid_legacy
        lda #191
        sta <ring_head
ring_valid_legacy
        tst <ring_head
        bne scroll_no_wrap
        inc <ring_wraps
ring_wrapped
        nop
scroll_no_wrap
        ldd <camera_y
        subd #1
        std <camera_y
        std <world_line
        lda <ring_head
        sta <target_line
        jsr render_world_line
        jsr draw_hud
        ldd <scroll_steps
        addd #1
        std <scroll_steps
no_camera_step
        jsr draw_player
        jsr publish_head
        ldd <camera_y
        bne game_return
        tst <scroll_due
        beq game_return
camera_stopped
        nop
game_return
        rts

game_combat_update
        ; Keep the last complete sprites visible while computing the next state.
        lda #1
        sta <render_deferred
        clr <render_refresh_pending
        tst <combat_status
        bne combat_skip_movement
        jsr terrain_check
        jsr move_player
        jsr terrain_after_move
combat_skip_movement
        clr <scroll_due
        tst <combat_status
        bne combat_scroll_ready
        lda <boss_state
        cmpa #2
        bhs combat_scroll_ready
        ldd <freeze_ticks
        bne combat_scroll_ready
        inc <scroll_clock
        lda <scroll_clock
        cmpa #4
        blo combat_scroll_ready
        clr <scroll_clock
        ldd <camera_y
        cmpd #48
        bls combat_scroll_ready
        inc <scroll_due
        dec <ring_head
        lda <ring_head
        cmpa #255
        bne ring_valid_combat
        lda #191
        sta <ring_head
ring_valid_combat
        tst <ring_head
        bne combat_no_ring_wrap
        inc <ring_wraps
combat_no_ring_wrap
        ldd <camera_y
        subd #1
        std <camera_y
        ldd <scroll_steps
        addd #1
        std <scroll_steps
combat_scroll_ready
        jsr combat_tick
        clr <render_deferred
        lda <respawn_pending
        beq combat_no_checkpoint_rebuild
        cmpa #2
        beq combat_game_over_intro
        jsr checkpoint_restore
        jmp game_return
combat_game_over_intro
        jsr game_over_to_intro
        jmp game_return
combat_no_checkpoint_rebuild
        lda EXIT_STATE
        cmpa #3
        bne render_begin
        jsr phase_advance
        jmp game_return
render_begin
        ; A stationary normal player over an unchanged background needs no
        ; framebuffer writes. Include old draw flags: vanished entities must
        ; still be erased, even when their current active byte is already zero.
        jsr render_can_retain
        tsta
        lbne combat_frame_complete
render_commit_begin
        ; Reverse erase order still matches the unchanged painter order.
        ; Render metadata +17..22 survives entity logic/slot reuse.
        jsr erase_player
        jsr combat_erase
        tst <scroll_due
        beq render_no_camera_line
        ldd <camera_y
        std <world_line
        lda <ring_head
        sta <target_line
        jsr render_world_line
        jsr draw_hud
render_no_camera_line
        tst <render_refresh_pending
        beq render_no_item_refresh
        jsr fixed_flush_refresh
render_no_item_refresh
        jsr hud_update
        jsr combat_draw
        jsr draw_player
        jsr publish_head
combat_frame_complete
        jmp game_return

render_can_retain
        lda <scroll_due
        ora <render_refresh_pending
        ora <hud_dirty
        ora <combat_status
        ora <player_moved
        ora <player_invul
        ora <ability_kind
        ora <shield_drawn
        ora <boss_state
        ora <boss_drawn
        bne render_retain_no
        tst <sprite_valid
        beq render_retain_no
        ; The pointer after drawing identifies the normal player mask family.
        ; This excludes a just-expired invisible-player image.
        ldd <sprite_pointer
        cmpd #$c120
        blo render_retain_no
        cmpd #$c480
        bhi render_retain_no
        ldx #ENTITIES
render_retain_entities
        lda ,x
        ora 17,x
        bne render_retain_no
        leax 32,x
        cmpx #ENTITY_END
        blo render_retain_entities
        lda #1
        rts
render_retain_no
        clra
        rts

move_player
        jsr item_movement_speed
        clr <player_moved
        lda <input_bits
        anda #$0f
        beq movement_done
        inc <player_moved
        ; Opposite directions cancel instead of introducing an axis bias.
        anda #$0c
        cmpa #$0c
        beq horizontal_done
        lda <input_bits
        bita #4
        beq test_right
        lda <player_x
        ldb <player_fx
        subd <move_vx
        stb <player_fx
        bcc store_left
        clra
store_left
        sta <player_x
test_right
        lda <input_bits
        bita #8
        beq horizontal_done
        lda <player_x
        ldb <player_fx
        addd <move_vx
        stb <player_fx
        cmpa #239
        bls store_right
        lda #239
store_right
        sta <player_x
horizontal_done
        lda <input_bits
        anda #3
        cmpa #3
        beq movement_done
        lda <input_bits
        bita #1
        beq test_down
        lda <player_y
        ldb <player_fy
        subd <move_vy
        stb <player_fy
        bcc store_up
        clra
store_up
        sta <player_y
test_down
        lda <input_bits
        bita #2
        beq movement_done
        lda <player_y
        ldb <player_fy
        addd <move_vy
        stb <player_fy
        cmpa #160
        bls store_down
        lda #160
store_down
        sta <player_y
movement_done
        tst <player_moved
        beq walking_idle
        inc <walk_clock
        lda <walk_clock
        lsra
        lsra
        anda #1
        sta <player_frame
        rts
walking_idle
        tst <player_frame
        beq idle_frame_ready
        inc <player_moved
idle_frame_ready
        clr <player_frame
        rts

; Scan real PIA matrix, allowing simultaneous axes and fire.
; Select K=keyboard, J=right stick, L=left stick; keyboard remains active.
poll_input
        clr <input_bits
        lda #$ff
        sta $ff02
        lda $ff00
        coma
        anda #$0f
        coma
        sta <button_mask
        ldx #key_rows
        lda #$fe
        sta <key_column
        ldb #8
keyboard_scan
        lda <key_column
        sta $ff02
keyboard_read
        lda $ff00
        coma
        anda #$7f
        anda <button_mask
        sta ,x+
        lda <key_column
        orcc #1
        rola
        sta <key_column
        decb
        bne keyboard_scan
        lda #$ff
        sta $ff02
        ldx #key_definitions
key_decode
        lda ,x+
        cmpa #$ff
        beq key_decode_done
        ldu #key_rows
        lda a,u
        anda ,x+
        beq key_not_down
        lda <input_bits
        ora ,x
        sta <input_bits
key_not_down
        leax 1,x
        bra key_decode
key_decode_done
        lda <key_rows+3
        bita #2
        beq test_j_mode
        clr <input_mode
test_j_mode
        lda <key_rows+2
        bita #2
        beq test_l_mode
        lda #1
        sta <input_mode
test_l_mode
        lda <key_rows+4
        bita #2
        beq input_mode_set
        lda #2
        sta <input_mode
input_mode_set
        tst <input_mode
        beq input_done
        jsr read_joystick
input_done
        rts

; Read two thresholds per analog axis (24/40 of 63): stable centre dead zone.
; Short critical section prevents audio FIRQ from changing the shared DAC.
read_joystick
        pshs cc
        orcc #$40
        lda $ff20
        sta <saved_dac
        lda $ff23
        sta <saved_sound
        anda #$f7
        sta $ff23
        lda $ff03
        anda #$f7
        ldb <input_mode
        cmpb #2
        bne joystick_right
        ora #8
joystick_right
        sta $ff03
        lda $ff01
        anda #$f7
        sta $ff01
        lda #96
        sta $ff20
        nop
joystick_x_low
        lda $ff00
        bmi joystick_x_high_test
        lda <input_bits
        ora #4
        sta <input_bits
        bra joystick_y_test
joystick_x_high_test
        lda #160
        sta $ff20
        nop
joystick_x_high
        lda $ff00
        bpl joystick_y_test
        lda <input_bits
        ora #8
        sta <input_bits
joystick_y_test
        lda $ff01
        ora #8
        sta $ff01
        lda #96
        sta $ff20
        nop
joystick_y_low
        lda $ff00
        bmi joystick_y_high_test
        lda <input_bits
        ora #1
        sta <input_bits
        bra joystick_buttons
joystick_y_high_test
        lda #160
        sta $ff20
        nop
joystick_y_high
        lda $ff00
        bpl joystick_buttons
        lda <input_bits
        ora #2
        sta <input_bits
joystick_buttons
        lda $ff00
        coma
        ldb <input_mode
        cmpb #2
        beq joystick_left_fire
        anda #5
        bra joystick_fire_test
joystick_left_fire
        anda #10
joystick_fire_test
        beq joystick_restore
        lda <input_bits
        ora #$10
        sta <input_bits
joystick_restore
        lda $ff01
        anda #$f7
        sta $ff01
        lda $ff03
        anda #$f7
        sta $ff03
        lda <saved_dac
        sta $ff20
        lda <saved_sound
        sta $ff23
        puls cc,pc

key_definitions
        ; column, row bit, input mask: arrows, WASD, space, pause, restart.
        fcb 3,8,1,4,8,2,5,8,4,6,8,8
        fcb 7,4,1,3,4,2,1,1,4,4,1,8
        fcb 7,8,$10,0,4,$20,2,4,$40,1,2,$80,$ff

; Map a physical raster line (0..383) and byte-column into the $8000 window.
framebuffer_pointer
        lda <span_row+1
        lsra
        lsra
        lsra
        lsra
        lsra
        lsra
        tst <span_row
        beq framebuffer_bank_ready
        adda #4
framebuffer_bank_ready
        sta <row_bank
        sta $ffa4
        lda <span_row+1
        lsra
        anda #$1f
        ora #$80
        ldb <span_x
        bcc framebuffer_even
        orb #$80
framebuffer_even
        tfr d,u
        rts

; X=source. A single physical copy; mirrored background rows are explicit.
write_span
        jsr framebuffer_pointer
write_span_current
        ldb <span_n
        cmpb #128
        beq write_full_scanline
write_span_loop
        lsrb
        stb <span_blocks
        bcc write_span_pairs
        lda ,x+
        sta ,u+
        tst <span_blocks
        beq write_span_done
write_span_pairs
        ldd ,x++
        std ,u++
        dec <span_blocks
        bne write_span_pairs
write_span_done
        rts
write_full_scanline
        lda #8
        sta <span_blocks
write_full_block
        ldd ,x
        std ,u
        ldd 2,x
        std 2,u
        ldd 4,x
        std 4,u
        ldd 6,x
        std 6,u
        ldd 8,x
        std 8,u
        ldd 10,x
        std 10,u
        ldd 12,x
        std 12,u
        ldd 14,x
        std 14,u
        leax 16,x
        leau 16,u
        dec <span_blocks
        bne write_full_block
        rts

; U is just beyond the copied span. Advance to its next raster line and
; switch the MMU only at an 8 KiB boundary, rather than recomputing per row.
advance_span
        ldb #128
        subb <span_n
        leau b,u
        cmpu #$a000
        blo advance_span_done
        leau -$2000,u
        inc <row_bank
        lda <row_bank
        sta $ffa4
advance_span_done
        rts

render_world_line
        ldd <world_line
        jsr campaign_map_row
        ldu #TILE_ROW
        ldb #32
copy_map_row
        lda ,x+
        sta ,u+
        decb
        bne copy_map_row
        jsr items_overlay_line
        lda #$0d
        sta $ffa6
        lda <world_line+1
        anda #7
        lsla
        lsla
        sta <tile_line
        lda #32
        sta <tiles_left
        ldx #TILE_ROW
        ldy #SCANLINE
render_tile_line
        lda ,x+
        ldb #32
        mul
        addd #$a000
        addb <tile_line
        adca #0
        tfr d,u
        ldd ,u++
        std ,y++
        ldd ,u
        std ,y++
        dec <tiles_left
        bne render_tile_line
        clr <span_x
        clr <span_row
        lda <target_line
        sta <span_row+1
        lda #128
        sta <span_n
        ldx #SCANLINE
        jsr write_span
        lda <target_line
        cmpa #192
        bhs no_mirror_line
        clra
        ldb <target_line
        addd #192
        std <span_row
        ldx #SCANLINE
        jsr write_span
no_mirror_line
        rts

draw_hud
        clra
        ldb <ring_head
        addd #VIEW_HEIGHT
        std <span_row
        clr <span_x
        lda #128
        sta <span_n
        lda #16
        sta <object_lines
        ldx #$d000
        jsr framebuffer_pointer
hud_row_loop
        jsr write_span_current
        jsr advance_span
        dec <object_lines
        bne hud_row_loop
        rts

erase_player
        tst <sprite_valid
        beq erase_return
        ldd <sprite_screen_row
        std <span_row
        lda <sprite_xbyte
        sta <span_x
        lda #9
        sta <span_n
        lda #16
        sta <object_lines
        ldx #SAVED_PLAYER
        jsr restore_span_select
        jsr framebuffer_pointer
erase_row_loop
        jsr [blit_fn]
        jsr advance_span
        dec <object_lines
        bne erase_row_loop
        clr <sprite_valid
erase_return
        jmp erase_shield

draw_player
        tst <combat_enabled
        beq player_visible
        lda <combat_status
        cmpa #1
        beq player_not_visible
        cmpa #2
        beq player_not_visible
        tst <player_invul
        beq player_visible
        lda <game_ticks+1
        bita #2
        beq player_visible
player_not_visible
        rts
player_visible
        jsr draw_shield
        clra
        ldb <ring_head
        addb <player_y
        adca #0
        std <sprite_screen_row
        std <span_row
        lda <player_x
        lsra
        sta <sprite_xbyte
        sta <span_x
        ldb <player_frame
        lslb
        lda <player_x
        anda #1
        stb <tiles_left
        adda <tiles_left
        lsla
        ldx #player_pointers
        pshs a
        lda <ability_kind
        cmpa #2
        bne player_mask_ready
        ldd <ability_timer
        cmpd #180
        bhs player_invisible_mask
        lda <game_ticks+1
        bita #4
        bne player_mask_ready
player_invisible_mask
        ldx #invisibility_pointers
player_mask_ready
        puls a
        ldx a,x
        stx <sprite_pointer
        ldy #SAVED_PLAYER
        lda #16
        sta <object_lines
        lda #9
        sta <span_n
        jsr framebuffer_pointer
draw_player_row
        ldx <sprite_pointer
        ; Same nine mask/color pairs, using the existing unrolled compositor.
        jsr masked_row_9
        stx <sprite_pointer
        jsr advance_span
        dec <object_lines
        bne draw_player_row
        lda #1
        sta <sprite_valid
        rts

player_pointers
        fdb $c000,$c120,$c240,$c360

publish_head
        clra
        ldb <ring_head
        lslb
        rola
        lslb
        rola
        lslb
        rola
        lslb
        rola
        addd #$c000
        std $ff9d
frame_complete
        rts
