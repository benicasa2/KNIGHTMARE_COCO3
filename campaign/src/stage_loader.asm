; Native 6809 resource loader laboratory, NOT the complete game campaign.
; Drive 0: Disk BASIC bootstrap. Drive 1: read-only CAMPAIGN.DSK resources.
; Physical bank 7: map, bank 12: packed tiles. $2000: original stage script.
; Streaming PackBits avoids retaining either the ROM or all eight stages in RAM.
        org $0900
stage_index rmb 1
stage_lsn rmb 2
sector_ptr rmb 2
disk_track rmb 1
wanted_track rmb 1
disk_sector rmb 1
resources_left rmb 1
bytes_left rmb 2
run_left rmb 1
repeat_byte rmb 1
loader_error rmb 1
loaded_stages rmb 1
auto_advance rmb 1
screen_line rmb 1
tile_scan rmb 1
tiles_left rmb 1
map_ptr rmb 2
camera_y rmb 2
key_value rmb 1

        org $3000
start
        orcc #$50
        lds #$2fff
        clra
        tfr a,dp
        sta $ffdf
        sta $ffd9
        clr $ff92
        clr $ff93
        ldx #$ffa0
        lda #8
map_fixed
        sta ,x+
        inca
        cmpa #16
        blo map_fixed
        ; CoCo 3 ROM NMI vector jumps through this RAM trampoline.
        lda #$3b
        sta $fefd
        lda #9
        tfr a,dp
        clr <stage_index
        clr <loaded_stages
        clr <loader_error
        clr <auto_advance
        lda #$4c
        sta $ff90
        clr $ff91
        lda #$80
        sta $ff98
        lda #$1a
        sta $ff99
        clr $ff9a
        clr $ff9c
        clr $ff9d
        clr $ff9e
        clr $ff9f
        ldx #palette
        ldu #$ffb0
        ldb #16
set_palette
        lda ,x+
        sta ,u+
        decb
        bne set_palette
        lda #$2a              ; motor, drive 1, double density, side 0
        sta $ff40
        lda $ff48
        clr $ff48             ; restore physical head, 6ms stepping
        jsr disk_wait
        clr <disk_track
next_stage
        jsr load_stage
        ldd #1760
        std <camera_y
        jsr render_stage
        inc <loaded_stages
stage_loaded
        nop                   ; debugger observes bytes actually loaded by CPU
waiting_for_space
        tst <auto_advance
        bne advance_stage
        jsr read_keys
        bita #$10
        bne stage_key_release
        bita #1
        bne preview_up
        bita #2
        bne preview_down
        bra waiting_for_space
preview_up
        ldd <camera_y
        beq waiting_for_space
        subd #8
        std <camera_y
        bra preview_render
preview_down
        ldd <camera_y
        cmpd #1760
        beq waiting_for_space
        addd #8
        std <camera_y
preview_render
        jsr render_stage
preview_release
        jsr read_keys
        tsta
        bne preview_release
        bra waiting_for_space
stage_key_release
        jsr read_keys
        tsta
        bne stage_key_release
advance_stage
        inc <stage_index
        lda <stage_index
        cmpa #8
        blo next_stage
all_stages_loaded
        clr $ff40
        bra all_stages_loaded

; Laboratory controls: arrows inspect the map in 8-pixel steps; SPACE next.
; There are deliberately no invented enemies or substitute bosses here.
read_keys
        clr <key_value
        lda #$f7
        sta $ff02
        lda $ff00
        bita #8
        bne key_not_up
        inc <key_value
key_not_up
        lda #$ef
        sta $ff02
        lda $ff00
        bita #8
        bne key_not_down
        lda <key_value
        ora #2
        sta <key_value
key_not_down
        lda #$7f
        sta $ff02
        lda $ff00
        bita #8
        bne key_not_space
        lda <key_value
        ora #$10
        sta <key_value
key_not_space
        lda #$ff
        sta $ff02
        lda <key_value
        rts

; Render the real loaded map/tiles into physical framebuffer banks 0..2.
render_stage
        lda #12
        sta $ffa5
        lda #7
        sta $ffa6
        ldd <camera_y
        lsra
        rorb
        lsra
        rorb
        lsra
        rorb
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
        addd #$c000
        std <map_ptr
        clr <screen_line
        clr <tile_scan
render_scanline
        lda <screen_line
        lsra
        lsra
        lsra
        lsra
        lsra
        lsra
        sta $ffa4
        lda <screen_line
        anda #63
        clrb
        lsra
        rorb
        addd #$8000
        tfr d,u
        ldx <map_ptr
        lda #32
        sta <tiles_left
render_tile
        clra
        ldb ,x+
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
        addb <tile_scan
        adca #$a0
        tfr d,y
        ldd ,y
        std ,u++
        ldd 2,y
        std ,u++
        dec <tiles_left
        bne render_tile
        lda <tile_scan
        adda #4
        anda #31
        sta <tile_scan
        bne render_same_row
        ldd <map_ptr
        addd #32
        std <map_ptr
render_same_row
        inc <screen_line
        lda <screen_line
        cmpa #192
        blo render_scanline
        rts

load_stage
        lda <stage_index
        lsla
        ldx #stage_directory
        ldd a,x
        std <stage_lsn
        ldd #$1100
        std <sector_ptr
        ; Validate KMST, version 1, requested stage and resource count.
        ldu #pack_header
        ldb #5
header_loop
        pshs b
        jsr stream_byte
        cmpa ,u+
        lbne bad_format
        puls b
        decb
        bne header_loop
        jsr stream_byte
        cmpa <stage_index
        lbne bad_format
        jsr stream_byte
        cmpa #3
        lbne bad_format
        sta <resources_left
resource_loop
        jsr stream_byte
        cmpa #$ff
        beq resource_script
        cmpa #7
        beq resource_bank
        cmpa #12
        lbne bad_format
resource_bank
        sta $ffa4
        ldy #$8000
        bra resource_length
resource_script
        ldy #$2000
resource_length
        jsr stream_byte
        sta <bytes_left
        jsr stream_byte
        sta <bytes_left+1
        ldd <bytes_left
        beq bad_format
        cmpy #$2000
        bne bank_length
        cmpd #$0800
        bhi bad_format
        bra unpack_control
bank_length
        cmpd #$2000
        bhi bad_format
unpack_control
        jsr stream_byte
        tsta
        bmi unpack_repeat
        inca
        sta <run_left
unpack_literal_loop
        jsr stream_byte
        jsr output_byte
        dec <run_left
        bne unpack_literal_loop
        bra unpack_continue
unpack_repeat
        anda #127
        inca
        sta <run_left
        jsr stream_byte
        sta <repeat_byte
unpack_repeat_loop
        lda <repeat_byte
        jsr output_byte
        dec <run_left
        bne unpack_repeat_loop
unpack_continue
        ldd <bytes_left
        bne unpack_control
        dec <resources_left
        bne resource_loop
        rts
output_byte
        pshs a
        ldd <bytes_left
        beq bad_format
        subd #1
        std <bytes_left
        puls a
        sta ,y+
        rts

bad_format
        lda #1
        bra disk_error
disk_timeout
        lda #2
        bra disk_error
disk_status_error
        ; Preserve WD2793 status separately from format and timeout codes.
        ora #$80
disk_error
        sta <loader_error
        lda #$24
        sta $ff9a
        lda #$d0
        sta $ff48
        lda $ff48
        clr $ff40
loader_failed
        bra loader_failed

; Returns A, preserves B/X/Y/U. Sector boundaries may occur in ANY run/header.
stream_byte
        pshs b,x,u
        ldx <sector_ptr
        cmpx #$1100
        blo stream_available
        jsr read_sector
        ldx #$1000
stream_available
        lda ,x+
        stx <sector_ptr
        puls b,x,u,pc

; Sequential logical sectors; directory allows reloading any stage independently.
read_sector
        ldd <stage_lsn
        clr <wanted_track
lsn_divide
        cmpd #18
        blo lsn_remainder
        subd #18
        inc <wanted_track
        bra lsn_divide
lsn_remainder
        incb
        stb <disk_sector
        lda <wanted_track
        cmpa #35
        bhs bad_format
        cmpa <disk_track
        beq head_ready
        sta $ff4b
        lda #$10              ; seek track specified in data register
        sta $ff48
        jsr disk_wait
        bita #$18             ; seek error / CRC
        bne disk_status_error
        lda <wanted_track
        sta <disk_track
head_ready
        lda <disk_track
        sta $ff49
        lda <disk_sector
        sta $ff4a
        lda #$80
        sta $ff48
        ldx #$1000
sector_byte
        ldu #0
wait_drq
        lda $ff48
        bita #2
        bne got_drq
        bita #1
        beq disk_status_error
        leau -1,u
        bne wait_drq
        lbra disk_timeout
got_drq
        lda $ff4b
        sta ,x+
        cmpx #$1100
        blo sector_byte
        jsr disk_wait
        bita #$1c             ; record not found, CRC, lost data
        lbne disk_status_error
        ldd <stage_lsn
        addd #1
        std <stage_lsn
        rts
disk_wait
        pshs x
        ldx #0
        ; Allow command register execution before testing BUSY.
        nop
        nop
wait_busy
        lda $ff48
        bita #1
        beq disk_wait_done
        leax -1,x
        bne wait_busy
        lbra disk_timeout
disk_wait_done
        puls x,pc
pack_header fcb 'K','M','S','T',1
palette
        include "../../src/palette.inc"
        include "stage_directory.inc"
program_end
        end start
