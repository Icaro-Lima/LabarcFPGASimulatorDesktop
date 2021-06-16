# usage: quartus_stp -t inst.tcl <file> <fpga>

set fn [lindex $argv 0]
set fp [open $fn r]
set c ""
set n 0

if [string match *.hex $fn] {
   set file_data [read $fp]
   set data [split $file_data "\n"]
   foreach line $data {
      if { [scan $line :02%4x00%4s n v] == 2 } then {
         puts $v
         set c $v$c
      }
   }
   set n [expr $n+1]
} elseif [string match *.101 $fn] {
   set file_data [read $fp]
   set data [split $file_data "\n"]
   foreach line $data {
      if {[string length $line] > 0 &&
          [string index $line 0] != "#" && [string index $line 0] != ";" && [string index $line 0] != "/" } then {
         set w ""
         while {[string length $line] > 0 && [string length $w] < 32} {
            set b [string index $line 0]
            if {$b=="0" || $b=="1"} {set w $w$b}
            set line [string range $line 1 end]
         }
         if {[string length $w] > 0} then {
            if {[string length $w] < 32} then {puts "too few bits in $w"} else {
               set v ""
               for {set i 0} {$i < 32} {set i [expr $i+4]} {
                   set nibble [string range $w $i [expr $i+3]]
                   set h 0
                   for {set j 0} {$j < 4} {set j [expr $j+1]} {
                      set b [string index $nibble $j]
                      if {$b=="1"} {set h [expr $h+(1<<(3-$j))]}
                   }
                   if {$h > 9} {set h [expr $h+7]}
                   set d [format "%c" [expr $h+48]]
                   set v $v$d
               }
               puts "$w = $v"
               set c $v$c
               set n [expr $n+1]
            }
         }
      }
   }
} elseif [string match *.dump $fn] {
   fconfigure $fp -translation binary
   set inData [read $fp]
   set n [string length $inData]
   for {set i 0} {$i < $n} {set i [expr $i+4]} {
      binary scan [string range $inData $i [expr $i+3]] H2H2H2H2 v0 v1 v2 v3
      puts -nonewline "$v0$v1$v2$v3 "
      set c $v0$v1$v2$v3$c
   }
   puts ""
   set n [expr $n / 4]
} elseif [string match *.objdump $fn] {
   set file_data [read $fp]
   set data [split $file_data "\n"]
   set n 0
   foreach line $data {
      set columns [split $line " "]
      foreach value $columns {
         if { [scan $value %s v] == 1 } then {
            scan $v %2s%2s%2s%2s v0 v1 v2 v3
            puts  $v3$v2$v1$v0
            set c $v3$v2$v1$v0$c
            set n [expr $n+1]
         }
      }
   }
} else {
   fconfigure $fp -translation binary
   set inData [read $fp]
   set n [string length $inData]
   for {set i 0} {$i < $n} {set i [expr $i+2]} {
      binary scan [string range $inData $i [expr $i+1]] H2H2 vh vl
      puts -nonewline "$vh$vl "
      set c $vh$vl$c
   }
   puts ""
   set n [expr $n / 2]
}

puts "<span style=color:blue>Info: $n instructions read successfully from $fn</span>"
close $fp

# Select hardware name according to command line argument.
set fpga [lindex $argv 1]
global usbblaster_name
# Test if it is either the USBBlaster or DE-SoC.
set hardware_name [lindex [get_hardware_names] $fpga]
if { [string match "USB-Blaster*" $hardware_name] } {
        set usbblaster_name $hardware_name
} elseif { [string match "DE-SoC*" $hardware_name] } {
        set usbblaster_name $hardware_name
} else {
        puts "Error: No suitable hardware at index $fpga"
        exit
}

if {$c == ""} { puts "Error: no data" } else {
   foreach device_name [get_device_names -hardware_name $usbblaster_name] {
        if { [string match "*4C*" $device_name] } {
                set d $device_name
        }
   }
   begin_memory_edit -device_name $d -hardware_name $usbblaster_name
   write_content_to_memory -instance_index 0 -start_address 0 -word_count $n -content $c -content_in_hex
   end_memory_edit
}


