set throttle off
set renderer none
set sound_driver null
set pause off
set pause_on_lost_focus off
set power on
set effects {3 11 77 71 14 18}
set index 0
set frame 0
set out [file normalize [file join [file dirname [info script]] .. reference]]
proc begin_effect {} {
 global effects index frame out f
 if {$index >= [llength $effects]} {exit; return}
 set id [lindex $effects $index]
 set frame 0
 set f [open "$out/sfx-$id-psg.bin" wb]
 # Isolated original audio engine. Disable interrupts, reset its state, request
 # one effect through $490C. $C030 spins; $C040 runs exactly one $49C0 tick.
 set init "f33100f02100e01101e00140003600edb0af3200e63e[format %02x $id]cd0c49f3c330c0"
 debug write_block memory 0xc000 [binary format H* $init]
 debug write_block memory 0xc030 [binary format H* c330c0]
 debug write_block memory 0xc040 [binary format H* cdc049c330c0]
 reg PC 0xc000
 after time 0.001 {reg PC 0xc040; after time 0.002 collect_effect}
}
proc collect_effect {} {
 global index frame f
 puts -nonewline $f [debug read_block {PSG regs} 0 16]
 incr frame
 if {$frame >= 180} {close $f; incr index; begin_effect; return}
 reg PC 0xc040
 after time 0.016688154 collect_effect
}
after time 5 begin_effect
after realtime 30 exit
