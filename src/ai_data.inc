; ROM-derived constants. Angles/velocities executed by the Z80 oracle.
ai_spawn_y:
        fcb 240,240,240,136,240,240,240,240,240,240,16,1,16,1,240,1
ai_burst_size:
        fcb 1,5,5,5,5,5,1,1,6,4,4,1,1,1,1,1
ai_burst_gap:
        fcb 1,5,5,5,7,7,1,1,8,16,6,1,1,1,1,1
ai_launch_x:
        fcb 40,80,127,128,176,216
ai_enemy_burst_kind:
        fcb 5,2,2,2,3,4,7,6,3,1,3,3,1,4,3,3
ai_enemy_shot_boxes:
        fcb 8,8,2,3,4,4,8,8,8,8,10,10,4,4,8,8
ai_launch_vx_hi:
        fcb 0,5,250,6,251,255
ai_shot_schedule_counts:
        fcb 3,3,3,3,3,3,3,3,3,3,3,3,3,3,5,4
ai_boss_target_offsets:
        fcb 0,0,0,224,16,32,208,16,48
ai_satellite_thresholds:
        fcb 58,255,170
ai_boss_weapon_boxes:
        fcb 4,1,4,1,13,1,13,1,5,1,13,1,10,2,10,2,5,2,5,2,5,2,5,2
ai_shot_schedules:
        fdb ai_schedule_0,ai_schedule_1,ai_schedule_2,ai_schedule_3,ai_schedule_4,ai_schedule_5,ai_schedule_6,ai_schedule_7,ai_schedule_8,ai_schedule_9,ai_schedule_10,ai_schedule_11,ai_schedule_12,ai_schedule_13,ai_schedule_14,ai_schedule_15
ai_schedule_0:
        fcb 16,80,144
ai_schedule_1:
        fcb 16,80,144
ai_schedule_2:
        fcb 16,40,72
ai_schedule_3:
        fcb 48,8,80
ai_schedule_4:
        fcb 11,40,72
ai_schedule_5:
        fcb 16,96,176
ai_schedule_6:
        fcb 80,160,240
ai_schedule_7:
        fcb 24,112,192
ai_schedule_8:
        fcb 16,56,144
ai_schedule_9:
        fcb 24,96,168
ai_schedule_10:
        fcb 32,80,128
ai_schedule_11:
        fcb 2,12,23
ai_schedule_12:
        fcb 16,80,144
ai_schedule_13:
        fcb 16,80,144
ai_schedule_14:
        fcb 16,48,80,112,144
ai_schedule_15:
        fcb 16,48,80,112
ai_vectors_8:
        fdb $03fc,$fffc
        fdb $03ec,$ff48
        fdb $03b0,$fe7c
        fdb $0350,$fdc8
        fdb $02d4,$fd2c
        fdb $0238,$fcb0
        fdb $0184,$fc50
        fdb $00b8,$fc14
        fdb $0004,$fc04
        fdb $ff48,$fc14
        fdb $fe7c,$fc50
        fdb $fdc8,$fcb0
        fdb $fd2c,$fd2c
        fdb $fcb0,$fdc8
        fdb $fc50,$fe7c
        fdb $fc14,$ff48
        fdb $fc04,$0004
        fdb $fc14,$00b8
        fdb $fc50,$0184
        fdb $fcb0,$0238
        fdb $fd2c,$02d4
        fdb $fdc8,$0350
        fdb $fe7c,$03b0
        fdb $ff48,$03ec
        fdb $0004,$03fc
        fdb $00b8,$03ec
        fdb $0184,$03b0
        fdb $0238,$0350
        fdb $02d4,$02d4
        fdb $0350,$0238
        fdb $03b0,$0184
        fdb $03ec,$00b8
ai_auxiliary_x:
        fcb 0,22,46,69,89,105,117,125,128,125,117,105,89,69,46,22,0,22,46,69,89,105,117,125,128,125,117,105,89,69,46,22
ai_tangents:
        fdb 1270,614,381,255,170,105,50,0
