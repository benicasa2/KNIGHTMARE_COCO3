; Factory Tandy S/SC: CoCo 0.895 MHz, AY 1.789 MHz, direct mode $AF.
; Music A/B and original effects C. No PCM/DDS and no speech commands.
; VBL builds targets; 1 kHz bounded FIRQ sends at most ONE protocol byte.
MUSIC_VOICE0 equ $0d80
MUSIC_VOICE1 equ $0d90
MUSIC_TRACK equ $0da4
SSC_TARGET equ $0dd0
SSC_SENT equ $0de0
SSC_PHASE equ $0deb
SSC_REG equ $0dec
SSC_VALUE equ $0ded
SSC_WAIT equ $0dee
SSC_FAULT equ $0def
SSC_EFFECT_TICKS equ $0df0
SSC_SCAN equ $0df1
SSC_LEFT equ $0df2

ssc_hw_reset
        lda #1
        sta $ff7d
        clr $ff7d
        rts
ssc_init
        pshs cc,d,x,y,u
        orcc #$50
        clr $ff93
        clr <sfx_priority
        jsr ssc_hw_reset
        ldx #SSC_TARGET
ssc_clear
        clr ,x+
        cmpx #SSC_SCAN+1
        blo ssc_clear
        ldx #SSC_SENT
        ldb #11
        lda #255
ssc_unknown
        sta ,x+
        decb
        bne ssc_unknown
        lda #$38
        sta SSC_TARGET+7
        ; Let the MCU finish its power/reset initialization (bounded).
        ldx #4096
ssc_reset_delay
        leax -1,x
        bne ssc_reset_delay
        lda #$af
        sta $ff7e
        ; A fitted cartridge asserts BUSY immediately after a host write.
        tst $ff7e
        bmi ssc_init_failed
        ldx #4096
ssc_init_ready
        tst $ff7e
        bmi ssc_init_ok
        leax -1,x
        bne ssc_init_ready
ssc_init_failed
        inc SSC_FAULT
ssc_init_ok
        jsr ssc_audio_route
        puls cc,d,x,y,u,pc

ssc_audio_route
        lda $ff01
        anda #$f7
        sta $ff01
        lda $ff03
        ora #8
        sta $ff03
        lda $ff23
        ora #8
        sta $ff23
        rts
ssc_kick
        tst SSC_FAULT
        bne ssc_kick_done
        lda #$20
        sta $ff93
ssc_kick_done
        rts

sound_stop
        pshs cc,a
        orcc #$50
        clr <sfx_priority
        clr SSC_EFFECT_TICKS
        clr SSC_TARGET+10
        lda #$38
        sta SSC_TARGET+7
        tst <music_enabled
        bne ssc_stop_effect_only
        clr SSC_TARGET+8
        clr SSC_TARGET+9
ssc_stop_effect_only
        jsr ssc_kick
        puls cc,a,pc
music_reset
        pshs cc,d
        orcc #$50
        clr <music_enabled
        lda <key_rows+5
        anda #2
        sta <music_previous_key
        clr <music_key_edge
        ldd #ENTITIES+7*32
        std <enemy_limit_end
        lda #255
        sta MUSIC_TRACK
        jsr sound_stop
        puls cc,d,pc
music_toggle
        pshs cc,d,x,y,u
        orcc #$50
        tst <music_enabled
        beq ssc_enable
        clr <music_enabled
        clr SSC_TARGET+8
        clr SSC_TARGET+9
        ldd #ENTITIES+7*32
        std <enemy_limit_end
        bra ssc_toggle_commit
ssc_enable
        tst SSC_FAULT
        bne ssc_toggle_done
        inc <music_enabled
        ldd #ENTITIES+5*32
        std <enemy_limit_end
        clr ENTITIES+5*32
        clr ENTITIES+6*32
        lda #255
        sta MUSIC_TRACK
        jsr music_tick
ssc_toggle_commit
        jsr ssc_kick
ssc_toggle_done
        puls cc,d,x,y,u,pc

music_tick
        tst SSC_FAULT
        lbne ssc_tick_done
        orcc #$40
        lda $ffa4
        pshs a
        lda #7
        sta $ffa4
        tst <music_enabled
        beq ssc_tick_effect
        clra
        tst <boss_state
        beq ssc_track_phase
        inca
ssc_track_phase
        pshs a
        lda <campaign_stage
        lsla
        adda ,s+
        cmpa MUSIC_TRACK
        beq ssc_tick_voices
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
ssc_tick_voices
        ldx #MUSIC_VOICE0
        ldy #SSC_TARGET
        jsr music_voice_tick
        ldx #MUSIC_VOICE1
        ldy #SSC_TARGET+2
        jsr music_voice_tick
ssc_tick_effect
        jsr ssc_effect_tick
        puls a
        sta $ffa4
ssc_tick_committed
        jsr ssc_audio_route
        jsr ssc_kick
ssc_tick_done
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
        bne ssc_voice_done
        jsr music_decode_event
        sta 6,x
        stb 11,x
        lda #3
        mul
        ldu #music_dictionary
        tst <campaign_stage
        beq ssc_dictionary_ready
        ldu #music_dictionary2
ssc_dictionary_ready
        leau d,u
        ldd ,u
        ; AY order is fine then coarse; 6809 words are coarse then fine.
        exg a,b
        std ,y
        lda 2,u
        sta 7,x
        cmpy #SSC_TARGET
        bne ssc_volume_b
        sta SSC_TARGET+8
        rts
ssc_volume_b
        sta SSC_TARGET+9
ssc_voice_done
        rts

; Same lossless decoder as DAC6/ORCH90. History is 256 two-byte events.
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

sfx_shot
        pshs a
        lda #1
        bra request_sfx
sfx_hit
        pshs a
        lda #2
        bra request_sfx
sfx_explosion
        pshs a
        lda #3
        bra request_sfx
sfx_player
        pshs a
        lda #4
        bra request_sfx
sfx_boss
        pshs a
        lda #5
        bra request_sfx
sfx_cycle
        pshs a
        lda #6
        bra request_sfx
sfx_pickup
        pshs a
        lda #7
request_sfx
        pshs cc,d,x
        orcc #$50
        tst SSC_FAULT
        bne sfx_denied
        tfr a,b
        cmpa #6
        bne ssc_not_cycle
        ldb #2
ssc_not_cycle
        cmpa #7
        bne ssc_priority_ready
        ldb #4
ssc_priority_ready
        cmpb <sfx_priority
        blo sfx_denied
        stb <sfx_priority
        deca
        lsla
        lsla
        ldx #sfx_table
        leax a,x
        ldd ,x
        std <sfx_ptr
        ldd 2,x
        std <sfx_end
        lda #1
        sta SSC_EFFECT_TICKS
sfx_denied
        puls cc,d,x
        puls a,pc
ssc_effect_tick
        tst <sfx_priority
        beq ssc_effect_done
        dec SSC_EFFECT_TICKS
        bne ssc_effect_done
        ldx <sfx_ptr
        cmpx <sfx_end
        bhs ssc_effect_end
        lda ,x+
        sta SSC_EFFECT_TICKS
        ldd ,x++
        std SSC_TARGET+4
        lda ,x+
        sta SSC_TARGET+6
        lda ,x+
        ora #$18
        sta SSC_TARGET+7
        lda ,x+
        sta SSC_TARGET+10
        stx <sfx_ptr
ssc_effect_done
        rts
ssc_effect_end
        clr <sfx_priority
        clr SSC_TARGET+10
        lda #$38
        sta SSC_TARGET+7
        rts

; No wait loops in the interrupt. BUSY is active low on EVERY byte.
audio_firq
        pshs d,x
        lda $ff93
        tst SSC_FAULT
        bne ssc_transport_idle
        tst $ff7e
        bmi ssc_transport_ready
        inc SSC_WAIT
        lda SSC_WAIT
        cmpa #120
        blo ssc_transport_return
        inc SSC_FAULT
        clr <music_enabled
        clr <sfx_priority
        ldd #ENTITIES+7*32
        std <enemy_limit_end
        jsr ssc_hw_reset
        bra ssc_transport_idle
ssc_transport_ready
        clr SSC_WAIT
        tst SSC_PHASE
        bne ssc_transport_value
        lda #11
        sta SSC_LEFT
        ldx #SSC_TARGET
        ldb SSC_SCAN
        abx
ssc_transport_scan
        lda ,x
        cmpa SSC_SENT-SSC_TARGET,x
        bne ssc_transport_register
        leax 1,x
        inc SSC_SCAN
        lda SSC_SCAN
        cmpa #11
        blo ssc_transport_next
        clr SSC_SCAN
        ldx #SSC_TARGET
ssc_transport_next
        dec SSC_LEFT
        bne ssc_transport_scan
ssc_transport_idle
        clr $ff93
        bra ssc_transport_return
ssc_transport_register
        sta SSC_VALUE
        lda SSC_SCAN
        sta SSC_REG
        sta $ff7e
        inc SSC_PHASE
        bra ssc_transport_return
ssc_transport_value
        lda SSC_VALUE
        sta $ff7e
        ldx #SSC_SENT
        ldb SSC_REG
        sta b,x
        clr SSC_PHASE
        inc SSC_SCAN
        lda SSC_SCAN
        cmpa #11
        blo ssc_transport_return
        clr SSC_SCAN
ssc_transport_return
        puls d,x
        rti

sfx_table
        fdb ssc_sfx_shot,ssc_sfx_shot_end
        fdb ssc_sfx_hit,ssc_sfx_hit_end
        fdb ssc_sfx_explosion,ssc_sfx_explosion_end
        fdb ssc_sfx_explosion,ssc_sfx_explosion_end
        fdb ssc_sfx_boss,ssc_sfx_boss_end
        fdb ssc_sfx_cycle,ssc_sfx_cycle_end
        fdb ssc_sfx_pickup,ssc_sfx_pickup_end
        include "ssc_data.inc"
