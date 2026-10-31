; Original KONAMI logo and title pixels; music remains disabled.
; Called only after the combat frame has finished its erase/update phase.
; Rebuilding the intro here must bypass the combat draw/HUD path entirely.
game_over_to_intro
        jsr combat_init
        clr <unlocked_exit
        clr <paused
        jsr items_init
        jmp intro_init

intro_init
        jsr music_reset
        jsr sound_stop
        ; MSX $44CC initializes VDP R7=$E4: transparent color 0 is BLUE.
        lda palette+4
        sta $ffb0
        sta $ff9a
        lda #1
        sta <scene_mode
        clr <intro_clock
        clr <ring_head
        clr <sprite_valid
        lda #168
        sta <intro_y
        ldx #SCANLINE
intro_black_line
        clr ,x+
        cmpx #SCANLINE+128
        blo intro_black_line
        ldd #0
        std <span_row
        clr <span_x
        lda #128
        sta <span_n
intro_clear_screen
        ldx #SCANLINE
        jsr write_span
        inc <span_row+1
        lda <span_row+1
        cmpa #192
        blo intro_clear_screen
        jsr intro_paint_logo
        jmp publish_head

intro_tick
        lda <input_edges
        bita #$40
        beq intro_no_reset
        jmp game_init
intro_no_reset
        bita #$10
        beq intro_no_fire
        lda <scene_mode
        cmpa #2
        bne intro_skip_logo
        jsr game_init
        jmp sfx_pickup
intro_skip_logo
        jsr intro_title
        jmp sfx_cycle
intro_no_fire
        lda <scene_mode
        cmpa #1
        bne intro_return
        inc <intro_clock
        lda <intro_clock
        cmpa #14
        bhi intro_hold
        ; Fourteen original 8-pixel steps, once every two MSX frames.
        jsr intro_erase_logo
        lda <intro_y
        suba #8
        sta <intro_y
        jsr intro_paint_logo
        rts
intro_hold
        cmpa #80
        beq intro_title
intro_return
        rts
intro_erase_logo
        ldx #SCANLINE
        bra intro_logo_common
intro_paint_logo
        ldx #intro_logo_data
intro_logo_common
        stx <fixed_tileptr
        clr <span_row
        lda <intro_y
        sta <span_row+1
        lda #40
        sta <span_x
        lda #48
        sta <span_n
        lda #24
        sta <intro_count
intro_logo_row
        ldx <fixed_tileptr
        jsr write_span
        ldx <fixed_tileptr
        cmpx #SCANLINE
        beq intro_logo_zero
        leax 48,x
        stx <fixed_tileptr
intro_logo_zero
        inc <span_row+1
        dec <intro_count
        bne intro_logo_row
        rts
intro_title
        ; $47BA sets R7=$E0 for the black title background.
        clr $ffb0
        clr $ff9a
        lda #2
        sta <scene_mode
        ldx #intro_title_rle
        ldd #0
        std <span_row
        clr <span_x
        jsr framebuffer_pointer
intro_rle_packet
        ldb ,x+
        stb <intro_count
        lda ,x+
intro_rle_run
        sta ,u+
        cmpu #$a000
        blo intro_rle_same_bank
        pshs a
        leau -$2000,u
        inc <row_bank
        lda <row_bank
        sta $ffa4
        puls a
intro_rle_same_bank
        dec <intro_count
        bne intro_rle_run
        cmpx #intro_title_end
        blo intro_rle_packet
        rts
