; Read-only WD2793 tile-bank loader. Drive 1: LEVELS.DSK.
; Only runs between scenes; video/audio IRQs are masked during disk access.
FD_LSN equ $0b00
FD_TRACK equ $0b02
FD_WANTED equ $0b03
FD_SECTOR equ $0b04
FD_LEFT equ $0b05
FD_END equ $0b06
FD_OLD equ $0b08
FD_ERROR equ $0b09
level_tiles_load
        pshs cc,d,x,y,u
        orcc #$50
        clr $ff93
        clr FD_ERROR
        lda $ffa4
        sta FD_OLD
        lda #12
        sta $ffa4
        lda #$3b
        sta $fefd
        lda #$2a
        sta $ff40
        lda $ff48
        clr $ff48
        jsr level_disk_wait
        bcs level_load_finish
        clr FD_TRACK
        lda <campaign_stage
        ldb #32
        mul
        std FD_LSN
        ldy #$8000
        lda #32
        sta FD_LEFT
level_load_sector
        jsr level_read_sector
        bcs level_load_finish
        dec FD_LEFT
        bne level_load_sector
level_load_finish
        lda #$d0
        sta $ff48
        lda $ff48
        clr $ff40
        lda FD_OLD
        sta $ffa4
        tst FD_ERROR
        beq level_load_ok
        lda #$24
        sta $ff9a
level_load_ok
        puls cc,d,x,y,u,pc
level_disk_fail
        lda #1
        sta FD_ERROR
        orcc #1
        rts
level_disk_wait
        pshs x
        ldx #0
        nop
        nop
level_wait_busy
        lda $ff48
        bita #1
        beq level_disk_ready
        leax -1,x
        bne level_wait_busy
        puls x
        lbra level_disk_fail
level_disk_ready
        puls x
        andcc #$fe
        rts
level_read_sector
        ldd FD_LSN
        clr FD_WANTED
level_lsn_track
        cmpd #18
        blo level_lsn_sector
        subd #18
        inc FD_WANTED
        bra level_lsn_track
level_lsn_sector
        incb
        stb FD_SECTOR
        lda FD_WANTED
        cmpa FD_TRACK
        beq level_head_ready
        sta $ff4b
        lda #$10
        sta $ff48
        jsr level_disk_wait
        bcs level_read_done
        bita #$18
        lbne level_disk_fail
        lda FD_WANTED
        sta FD_TRACK
level_head_ready
        lda FD_TRACK
        sta $ff49
        lda FD_SECTOR
        sta $ff4a
        lda #$80
        sta $ff48
        tfr y,d
        addd #256
        std FD_END
level_sector_byte
        ldu #0
level_wait_drq
        lda $ff48
        bita #2
        bne level_got_drq
        bita #1
        lbeq level_disk_fail
        leau -1,u
        bne level_wait_drq
        lbra level_disk_fail
level_got_drq
        lda $ff4b
        sta ,y+
        cmpy FD_END
        blo level_sector_byte
        jsr level_disk_wait
        bcs level_read_done
        bita #$1c
        lbne level_disk_fail
        ldd FD_LSN
        addd #1
        std FD_LSN
        andcc #$fe
level_read_done
        rts
