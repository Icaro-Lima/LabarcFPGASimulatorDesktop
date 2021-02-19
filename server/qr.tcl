##############################################################################################
############################# vJTAG Socket nterface ##########################################
##############################################################################################

# http://idlelogiclabs.com/2012/04/15/talking-to-the-de0-nano-using-the-virtual-jtag-interface/

#This portion of the script is derived from some of the examples from Altera

puts stderr "Info: Running Quartus II 64-Bit SignalTap II"

global usbblaster_name
global test_device
# Select hardware name according to command line argument.
set fpga [lindex $argv 0]
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

puts "Info: Select JTAG chain connected to $usbblaster_name.";

# List all devices on the chain, and select the first CYCLON device on the chain.

foreach device_name [get_device_names -hardware_name $usbblaster_name] {
#	puts $device_name
	if { [string match "*4C*" $device_name] } {
		set test_device $device_name
	}
}
puts "Info: Select device: $test_device.";


# Open device 
proc openport {} {
	global usbblaster_name
        global test_device
	open_device -hardware_name $usbblaster_name -device_name $test_device
}


# Close device.  Just used if communication error occurs
proc closeport { } {
	catch {device_unlock}
	catch {close_device}
}

proc get_data {send_data} {
	openport   
	device_lock -timeout 10000

	device_virtual_ir_shift -instance_index 0 -ir_value 1 -no_captured_ir_value
	set tdi [device_virtual_dr_shift -dr_value $send_data -instance_index 0  -length 8]
	# Set IR back to 0, which is bypass mode
	device_virtual_ir_shift -instance_index 0 -ir_value 0 -no_captured_ir_value
  
	closeport

	set tdibin [binary format B8 $tdi]
	binary scan $tdibin H2 tdihex
	return $tdihex
}


#Code Dervied from Tcl Developer Exchange – http://www.tcl.tk/about/netserver.html

proc Start_Server {fpga} {
   set port [expr 2540 + $fpga]
   set s [socket -server ConnAccept $port]
   puts "Info: Running Socket Server on port $port"
   set h [info hostname]
   set f [expr [string first "." $h] - 1]
   puts stderr "<h4>Agora digite: ./remote [string range $h 0 $f] $fpga</h4>"
   flush stderr   
   vwait forever
}

proc ConnAccept {sock addr port} {
   global conn

# Record the client’s information

   puts stderr "Info: Accept connection from $addr $port"
   flush stderr
   set conn(addr,$sock) [list $addr $port]

# Ensure that each "puts" by the server
# results in a network transmission

   fconfigure $sock -buffering line

# Set up a callback for when the client sends data

   fileevent $sock readable [list IncomingData $sock]
}


proc IncomingData {sock} {
   global conn

# Check end of file or abnormal connection drop,
# then write the data to the vJTAG

   if {[eof $sock] || [catch {gets $sock line}]} {
      close $sock
      puts stderr "Info: Close connection from $conn(addr,$sock)"
      flush stderr   
      unset conn(addr,$sock)
      exit 0
   } else {
# Before the connection is closed we get an emtpy data transmission. Let’s check for it and trap it
      set data_len [string length $line]
      if {$data_len != "0"} then {
# $line may be a command to change SWI or a command to read LED
# however, a command to change SWI also reads LED
         get_data $line
# Send the vJTAG Commands to Update the LED and SEG
# the data returned are always for the command of the previous call 
	 set led [get_data "00100001"]
	 set seg [get_data "00000000"]
         puts $sock $seg$led
      }
  }
}

#Start that Server
Start_Server $fpga

