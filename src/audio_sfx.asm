; Original PSG effects rendered offline to unsigned 8-bit PCM at 3924.94 Hz.
; One priority PCM effect; optional native music is mixed in music_firq.
sound_stop
        pshs cc,a
        orcc #$40
        clr $ff93
        tst <music_enabled
        beq sound_stop_no_music
        lda #$20
        sta $ff93
sound_stop_no_music
        clr <sfx_priority
        lda #128
        ifne ORCH90
        sta $ff7a
        sta $ff7b
        else
        sta $ff20
        endc
        puls cc,a,pc

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
        orcc #$40
        tfr a,b
        cmpa #6
        bne sfx_not_cycle
        ldb #2
sfx_not_cycle
        cmpa #7
        bne sfx_priority_ready
        ldb #4
sfx_priority_ready
        cmpb <sfx_priority
        blo sfx_denied
        stb <sfx_priority
        jsr music_fast_rate
        deca
        lsla
        lsla
        ldx #sfx_table
        leax a,x
        ldd ,x
        std <sfx_ptr
        ldd 2,x
        std <sfx_end
        lda $ff93
        lda #$20
        sta $ff93
sfx_denied
        puls cc,d,x
        puls a,pc

audio_firq
        pshs d,x
        lda $ff93
        tst <music_enabled
        lbne music_firq
        ldx <sfx_ptr
        lda $ffa4
        pshs a
        lda #7
        sta $ffa4
        lda ,x+
        puls b
        stb $ffa4
        stx <sfx_ptr
output_sample
        ifne ORCH90
        sta $ff7a
        sta $ff7b
        else
        anda #$fc
        sta $ff20
        endc
        cmpx <sfx_end
        blo sfx_sample_done
        clr $ff93
        clr <sfx_priority
        lda #128
        ifne ORCH90
        sta $ff7a
        sta $ff7b
        else
        sta $ff20
        endc
sfx_sample_done
        puls d,x
        rti

sfx_table
        fdb pcm_shot,pcm_shot_end
        fdb pcm_hit,pcm_hit_end
        fdb pcm_explosion,pcm_explosion_end
        fdb pcm_explosion,pcm_explosion_end
        fdb pcm_boss,pcm_boss_end
        fdb pcm_cycle,pcm_cycle_end
        fdb pcm_pickup,pcm_pickup_end
        include "pcm_locations.inc"
