set throttle off
set renderer none
set sound_driver null
set pause off
set pause_on_lost_focus off
set power on
set out [file normalize "reference"]
set log [open "$out/capture.log" w]
puts $log [debug list]
puts $log [help debug watchpoint]
puts $log [help debug watchpoint create]
puts $log [help debug breakpoint create]
puts $log "status: power=$power pause=$pause"
flush $log
set hits [open "$out/executed-addresses.tsv" w]
puts $hits "emulated_seconds address_hex"
source tools/observed_breakpoints.tcl
set psglog [open "$out/psg-writes.tsv" w]
puts $psglog "emulated_seconds register value pc_hex"
set psgaddr 0
debug watchpoint create -type write_io -address 0xa0 -command {set ::psgaddr [expr {$::wp_last_value & 15}]}
debug watchpoint create -type write_io -address 0xa1 -command {puts $::psglog "[machine_info time] $::psgaddr $::wp_last_value [format %04x [reg PC]]"}
set frames [open "$out/psg-frames.bin" wb]
set frame_count 0
proc onframe {} {
 global frames frame_count
 puts -nonewline $frames [debug read_block {PSG regs} 0 16]
 incr frame_count
 if {$frame_count < 1800} {after time 0.016688154 onframe}
}
after time 0.016688154 onframe
proc capture {label} {
 global out log
 foreach dev {memory VRAM {VDP regs} {PSG regs}} {
  if {[catch {set n [debug size $dev]} e]} {puts $log "$dev: $e"; continue}
  set name [string map {" " "_"} $dev]
  set f [open "$out/$label-$name.bin" wb]
  puts -nonewline $f [debug read_block $dev 0 $n]
  close $f
 }
 puts $log "$label: [machine_info time] [reg PC]"
 flush $log
}
after time 5 {capture title; keymatrixdown 8 1}
after time 5.2 {keymatrixup 8 1}
after time 6 {keymatrixdown 8 1}
after time 6.2 {keymatrixup 8 1}
after time 7 {capture start}
after time 9 {capture play}
after time 10 {record start -audioonly "$out/MSX-reference.wav"}
after time 10.1 {keymatrixdown 8 1}
after time 29.5 {keymatrixup 8 1}
after time 12 {capture later}
after time 18 {capture action}
after time 30 {capture end; record stop; close $log; close $psglog; close $frames; close $hits; exit}
after realtime 22 {exit}
