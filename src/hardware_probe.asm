; Knightmare / CoCo 3: native first-stage combat prototype.
; 6809 only, 128 KiB, RGB palette, 256x192x16, DAC6/ORCH90/factory SSC.
; Original art; M toggles two-voice music; PCM or native AY event effects.
; LWASM: --format=decb --define=ORCH90=0 (or 1), SOUND_SSC=0 (or 1).
        ifndef SOUND_SSC
SOUND_SSC equ 0
        endc
        org $0c00
state_begin
frame_due rmb 1
frames_played rmb 2
sfx_ptr rmb 2
sfx_end rmb 2
sfx_priority rmb 1
        include "game_state.inc"
        include "combat_state.inc"
        include "items_state.inc"
music_enabled rmb 1
music_previous_key rmb 1
music_key_edge rmb 1
music_bits rmb 1
music_phase0 rmb 2
music_phase1 rmb 2
music_step0 rmb 2
music_step1 rmb 2
enemy_limit_end rmb 2
music_half_rate rmb 1
state_end

        org $3000
start
        orcc #$50
        lds #$2fff
        clra
        tfr a,dp
        sta $ffdf
        ifne SOUND_SSC
        sta $ffd8
        else
        sta $ffd9
        endc
        lda #$4c
        sta $ff90
        clr $ff91
        clr $ff92
        clr $ff93
        ldx #$ffa0
        lda #$38
map_default
        sta ,x+
        inca
        cmpa #$40
        blo map_default
        lda #6
        sta $ffa7
        lda #$80
        sta $ff98
        lda #$1a
        sta $ff99
        lda #$c0
        sta $ff9d
        clr $ff9e
        clr $ff9c
        clr $ff9f
        clr $ff9a
        ldx #palette
        ldu #$ffb0
        ldb #16
copy_palette
        lda ,x+
        sta ,u+
        decb
        bne copy_palette
        ldx #state_begin
clear_state
        clr ,x+
        cmpx #state_end
        blo clear_state
        lda #$0c
        tfr a,dp
        ifne SOUND_SSC
        jsr ssc_init
        endc
        jsr intro_init
        jsr sound_stop
        ; PIA interrupts off; GIME vertical IRQ clocks gameplay independently.
        lda $ff01
        anda #$f6
        sta $ff01
        lda $ff03
        anda #$f6
        sta $ff03
        lda $ff21
        anda #$f7
        sta $ff21
        lda $ff23
        ifne ORCH90
        anda #$f7
        else
        ora #$08
        endc
        sta $ff23
        ; PIA1 PA bits 7..2 output, low two input/control bits preserved.
        lda $ff21
        anda #$fb
        sta $ff21
        lda #$fc
        sta $ff20
        lda $ff21
        ora #$04
        sta $ff21
        ; $FFF6 is always ROM on the CoCo 3 and vectors through RAM $FEF4.
        lda #$7e
        sta $fef4
        ldd #audio_firq
        std $fef5
        lda #$7e
        sta $fef7
        ldd #video_irq
        std $fef8
        lda #$7c
        sta $ff90
        lda #$20
        sta $ff91
        clr $ff93
        lda #8
        sta $ff92
        ; PCM: 910+2 ticks. SSC byte transport: 3578+2 ticks.
        ifne SOUND_SSC
        ldb #$fa
        else
        ldb #$8e
        endc
        stb $ff95
        ifne SOUND_SSC
        lda #$0d
        else
        lda #$03
        endc
        sta $ff94
        andcc #$af
main_loop
        tst <frame_due
        beq main_loop
        orcc #$10
        clr <frame_due
        andcc #$ef
        jsr game_tick
        bra main_loop

; A full IRQ automatically saves/restores all 6809 registers.
video_irq
        lda $ff92
        lda #1
        sta <frame_due
        ldd <frames_played
        addd #1
        std <frames_played
        jsr music_tick
        rti

        ifne SOUND_SSC
        include "audio_ssc.asm"
        else
        include "audio_sfx.asm"
        include "music.asm"
        endc

        include "campaign_maps.asm"
        include "level_disk.asm"
        include "phases.asm"
        include "cutscene.asm"
        include "gameplay.asm"
        include "combat.asm"
        include "items.asm"
        include "weapons.asm"
        include "masked_rows.inc"
        include "raw_rows.inc"
        include "restore_spans.inc"
palette
        include "palette.inc"
program_end

; Permanent bank 6 at E000. Interrupt trampolines FEF4..FFFF are reserved.
        org $e000
aux_begin
        include "intro.asm"
intro_title_rle
        includebin "../assets/title.rle"
intro_title_end
intro_logo_data
        includebin "../assets/intro-logo.4bpp"
campaign_blob
        includebin "../assets/campaign-maps.rom.bin"
campaign_blob_end
aux_end
        end start
