; Two original PSG tone voices, compact events and 16-bit phase DDS.
; Native runtime only. Buffers $0500/$0600 are owned by the game, no BIOS use.
MUSIC_VOICE0 equ $0d80
MUSIC_VOICE1 equ $0d90
MUSIC_MIX equ $0da0
MUSIC_TRACK equ $0da4
MUSIC_SAMPLE equ $0da5
; voice: ptr,end,loop (words), ticks,amp,writepos,matchleft,offset,id,ringbase.

music_reset
        pshs cc,a,b
        orcc #$50
        clr <music_enabled
        jsr music_fast_rate
        lda <key_rows+5
        anda #2
        sta <music_previous_key
        clr <music_key_edge
        ldd #ENTITIES+7*32
        std <enemy_limit_end
        jsr sound_stop
        puls cc,a,b,pc

music_toggle
        pshs cc,d,x,y,u
        orcc #$50
        tst <music_enabled
        beq music_enable
        clr <music_enabled
        jsr music_fast_rate
        ldd #ENTITIES+7*32
        std <enemy_limit_end
        tst <sfx_priority
        bne music_toggle_done
        clr $ff93
        lda #128
        jsr music_output
        bra music_toggle_done
music_enable
        inc <music_enabled
        ldd #ENTITIES+5*32
        std <enemy_limit_end
        ; Preserve their old draw metadata until the render commit erases them.
        clr ENTITIES+5*32
        clr ENTITIES+6*32
        lda #$ff
        sta MUSIC_TRACK
        clr <music_phase0
        clr <music_phase0+1
        clr <music_phase1
        clr <music_phase1+1
        jsr music_tick
        lda $ff93
        lda #$20
        sta $ff93
music_toggle_done
        puls cc,d,x,y,u,pc

; Called at vertical IRQ rate, regardless of main-loop rendering delays.
music_tick
        tst <music_enabled
        lbeq music_tick_done
        orcc #$40
        clra
        tst <boss_state
        beq music_track_ready
        inca
music_track_ready
        pshs a
        lda <campaign_stage
        lsla
        adda ,s+
        cmpa MUSIC_TRACK
        beq music_tick_voices
        sta MUSIC_TRACK
        ldb #12
        mul
        addd #music_tracks
        tfr d,u
        ldx #MUSIC_VOICE0
        jsr music_init_voice
        leau 2,u
        ldx #MUSIC_VOICE1
        jsr music_init_voice
        ldd #$0500
        std MUSIC_VOICE0+12
        ldd #$0700
        std MUSIC_VOICE1+12
music_tick_voices
        ldx #MUSIC_VOICE0
        ldy #music_step0
        jsr music_voice_tick
        ldx #MUSIC_VOICE1
        ldy #music_step1
        jsr music_voice_tick
        lda MUSIC_VOICE0+7
        nega
        suba MUSIC_VOICE1+7
        sta MUSIC_MIX
        adda MUSIC_VOICE1+7
        adda MUSIC_VOICE1+7
        sta MUSIC_MIX+1
        lda MUSIC_VOICE0+7
        suba MUSIC_VOICE1+7
        sta MUSIC_MIX+2
        adda MUSIC_VOICE1+7
        adda MUSIC_VOICE1+7
        sta MUSIC_MIX+3
        jsr music_select_rate
music_tick_done
        rts
music_init_voice
        ldd ,u
        std ,x
        ldd 4,u
        std 2,x
        ldd 8,u
        std 4,x
        lda #1
        sta 6,x
        clr 7,x
        clr 8,x
        clr 9,x
        rts
music_voice_tick
        dec 6,x
        bne music_voice_done
        jsr music_decode_event
        sta 6,x
        stb 11,x
        lda #3
        mul
        ldu #music_dictionary
        tst <campaign_stage
        beq music_dictionary_ready
        ldu #music_dictionary2
music_dictionary_ready
        leau d,u
        ldd ,u
        std 14,x
        tst <music_half_rate
        beq music_voice_base_step
        lslb
        rola
music_voice_base_step
        std ,y
        lda 2,u
        sta 7,x
music_voice_done
        rts

; Literal [duration,state]; match [0,event-count,byte-distance].
; History positions are even and wrap modulo 256; overlaps are supported.
music_decode_event
        tst 9,x
        beq music_decode_token
music_decode_match
        ldb 8,x
        subb 10,x
        clra
        lslb
        rola
        ldu 12,x
        leau d,u
        ldd ,u
        dec 9,x
        bra music_store_event
music_decode_token
        ldu ,x
        cmpu 2,x
        blo music_read_token
        ldu 4,x
music_read_token
        lda ,u+
        beq music_read_match
        ldb ,u+
        stu ,x
        bra music_store_event
music_read_match
        lda ,u+
        sta 9,x
        lda ,u+
        sta 10,x
        stu ,x
        bra music_decode_match
music_store_event
        pshs d
        clra
        ldb 8,x
        lslb
        rola
        ldu 12,x
        leau d,u
        puls d
        std ,u
        inc 8,x
        rts

; FIRQ entry registers D/X are already saved. The original PCM path is kept
; intact with music off. With music on, signed music is added without clipping
; to the bounded PCM effects (their full peak + 24 stays inside 0..255).
music_firq
        ldd <music_phase0
        addd <music_step0
        std <music_phase0
        asla
        rol <music_bits
        ldd <music_phase1
        addd <music_step1
        std <music_phase1
        asla
        rol <music_bits
        lda <music_bits
        anda #3
        ldx #MUSIC_MIX
        lda a,x
        sta MUSIC_SAMPLE
        tst <sfx_priority
        beq music_only_sample
        ldx <sfx_ptr
        lda $ffa4
        pshs a
        lda #7
        sta $ffa4
        lda ,x+
        puls b
        stb $ffa4
        stx <sfx_ptr
        adda MUSIC_SAMPLE
        jsr music_output
        cmpx <sfx_end
        blo music_sample_done
        clr <sfx_priority
        jsr music_select_rate
        bra music_sample_done
music_only_sample
        adda #128
        jsr music_output
music_sample_done
        puls d,x
        rti
music_output
        ifne ORCH90
        sta $ff7a
        sta $ff7b
        else
        anda #$fc
        sta $ff20
        endc
        rts

; Music alone uses half the sample IRQ rate when both fundamentals fit.
; Effects always run at the original 3924.94 Hz. DDS steps scale with rate.
music_select_rate
        tst <sfx_priority
        bne music_fast_rate
        tst MUSIC_VOICE0+7
        beq music_check_voice1
        ldd MUSIC_VOICE0+14
        cmpd #16384
        bhi music_fast_rate
music_check_voice1
        tst MUSIC_VOICE1+7
        beq music_slow_rate
        ldd MUSIC_VOICE1+14
        cmpd #16384
        bhi music_fast_rate
music_slow_rate
        tst <music_half_rate
        bne music_rate_done
        pshs cc,d
        orcc #$50
        inc <music_half_rate
        ldd MUSIC_VOICE0+14
        lslb
        rola
        std <music_step0
        ldd MUSIC_VOICE1+14
        lslb
        rola
        std <music_step1
        ldb #$1e
        stb $ff95
        lda #$07
        sta $ff94
        puls cc,d,pc
music_fast_rate
        tst <music_half_rate
        beq music_rate_done
        pshs cc,d
        orcc #$50
        clr <music_half_rate
        ldd MUSIC_VOICE0+14
        std <music_step0
        ldd MUSIC_VOICE1+14
        std <music_step1
        ldb #$8e
        stb $ff95
        lda #$03
        sta $ff94
        puls cc,d,pc
music_rate_done
        rts

        include "music_data.inc"
