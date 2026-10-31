; Original 4x4 dictionary/repetition/mirror decoder, one 128-byte band.
; The active map is cached in CPU $0400..$047f, not expanded over bank 7.
MAP_CACHE equ $0400
MAP_LIST equ $0d20
MAP_BITS_END equ $0d22
MAP_CODE equ $0d24
MAP_BITS_PTR equ $0d26
MAP_SEG equ $0d28
MAP_BAND equ $0d29
MAP_BITS equ $0d2a
MAP_GROUP equ $0d2b
MAP_MIRROR equ $0d2c
MAP_ROWS equ $0d2d
MAP_COLS equ $0d2e
MAP_BAND_DEST equ $0d30
MAP_GLOBAL equ $0d32
MAP_CACHED equ $0d33
MAP_STAGE equ $0d34
MAP_PIXEL equ $0d35
MAP_DELTA equ campaign_blob-$99cd
campaign_map_load
        lda #255
        sta MAP_CACHED
        sta MAP_STAGE
        rts
; D = world raster line, X = pointer to the requested 32-tile row.
campaign_map_row
        pshs d,y,u
        stb MAP_PIXEL
        lsra
        rorb
        lsra
        rorb
        lsra
        rorb
        lsra
        rorb
        lsra
        rorb
        cmpb MAP_CACHED
        bne campaign_row_miss
        lda <campaign_stage
        cmpa MAP_STAGE
        beq campaign_row_ready
campaign_row_miss
        tfr b,a
        jsr campaign_decode_band
        jsr campaign_patch_boss
campaign_row_ready
        clra
        ldb MAP_PIXEL
        andb #24
        lslb
        lslb
        addd #MAP_CACHE
        tfr d,x
        puls d,y,u,pc
; A = global four-tile-row band 0..60; output is the raw MSX band.
campaign_decode_band
        pshs d,x,y,u
        sta MAP_GLOBAL
        sta MAP_CACHED
        lda <campaign_stage
        sta MAP_STAGE
        lsla
        lsla
        ldx #campaign_blob
        leax a,x
        jsr campaign_read_pointer
        std MAP_LIST
        leax 2,x
        jsr campaign_read_pointer
        std MAP_BITS_END
        lda #9
        sta MAP_SEG
        lda MAP_GLOBAL
campaign_choose_segment
        cmpa #7
        blo campaign_segment_found
        suba #6
        dec MAP_SEG
        bra campaign_choose_segment
campaign_segment_found
        sta MAP_BAND
        lda MAP_SEG
        ldb #6
        mul
        std MAP_BITS_PTR
        ldd MAP_BITS_END
        subd MAP_BITS_PTR
        tfr d,y
        lda MAP_SEG
        lsla
        ldx MAP_LIST
        leax a,x
        jsr campaign_read_pointer
        tfr d,x
        ldb MAP_BAND
        beq campaign_band_selected
campaign_skip_band
        lda ,y+
        sta MAP_BITS
        lda #8
        sta MAP_COLS
campaign_popcount
        asl MAP_BITS
        bcc campaign_no_code_advance
        leax 1,x
campaign_no_code_advance
        dec MAP_COLS
        bne campaign_popcount
        decb
        bne campaign_skip_band
campaign_band_selected
        stx MAP_CODE
        lda ,y
        sta MAP_BITS
        clr MAP_GROUP
        ldu #MAP_CACHE
campaign_group
        ldx MAP_CODE
        lda ,x
        sta MAP_MIRROR
        anda #$7f
        ldb #2
        mul
        ldx #campaign_blob+$20
        leax d,x
        jsr campaign_read_pointer
        tfr d,y
        lda #4
        sta MAP_ROWS
        tst MAP_MIRROR
        bmi campaign_mirror_block
campaign_normal_row
        lda #4
        sta MAP_COLS
campaign_normal_pixel
        lda ,y+
        sta ,u+
        dec MAP_COLS
        bne campaign_normal_pixel
        leau 28,u
        dec MAP_ROWS
        bne campaign_normal_row
        leau -124,u
        bra campaign_next_group
campaign_mirror_block
        leau 3,u
campaign_mirror_row
        lda #4
        sta MAP_COLS
campaign_mirror_pixel
        lda ,y+
        bita #$f0
        beq campaign_mirror_store
        cmpa #$a0
        blo campaign_mirror_add
        suba #$60
        bra campaign_mirror_store
campaign_mirror_add
        adda #$60
campaign_mirror_store
        sta ,u
        leau -1,u
        dec MAP_COLS
        bne campaign_mirror_pixel
        leau 36,u
        dec MAP_ROWS
        bne campaign_mirror_row
        leau -127,u
campaign_next_group
        asl MAP_BITS
        bcc campaign_repeat_code
        ldx MAP_CODE
        leax 1,x
        stx MAP_CODE
campaign_repeat_code
        inc MAP_GROUP
        lda MAP_GROUP
        cmpa #8
        lblo campaign_group
        puls d,x,y,u,pc
campaign_read_pointer
        ldb ,x
        lda 1,x
        addd #MAP_DELTA
        rts
; Remove only the first guardian's baked-in face; the mobile body is separate.
campaign_patch_boss
        tst <campaign_stage
        bne campaign_patch_done
        lda MAP_GLOBAL
        ldb #4
        cmpa #2
        beq campaign_patch_rows
        cmpa #3
        bne campaign_patch_done
        ldb #1
campaign_patch_rows
        ldu #MAP_CACHE+10
campaign_patch_row
        lda ,u
        sta 4,u
        sta 5,u
        sta 6,u
        sta 7,u
        sta 8,u
        leau 32,u
        decb
        bne campaign_patch_row
campaign_patch_done
        rts
