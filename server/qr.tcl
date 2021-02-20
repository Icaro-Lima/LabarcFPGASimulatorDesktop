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
   puts stderr "Info: Starting JTAG server"
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
# the data returned are always for the command of the previous call 
#        if lcd_a and lcd_b are requested
	 if { [string range $line 0 3] == "0000"} then {           
            puts $sock "[get_data "00000001"][get_data "00000010"][get_data "00000011"][get_data "00000100"][get_data "00000101"][get_data "00000110"][get_data "00000111"][get_data "00001000"][get_data "00001001"][get_data "00001010"][get_data "00001011"][get_data "00001100"][get_data "00001101"][get_data "00001110"][get_data "00001111"][get_data "00000000"]"
         } elseif { [string range $line 0 3] == "0011"} then {
            puts $sock "[get_data "00111110"][get_data "00111101"][get_data "00111100"][get_data "00111011"][get_data "00111010"][get_data "00111001"][get_data "00111000"][get_data "00110111"][get_data "00110110"][get_data "00110101"][get_data "00110100"][get_data "00110011"][get_data "00110010"][get_data "00110001"][get_data "00110000"][get_data "00000000"]"
#              if pc is requested, get also instruction up to flags
         } elseif { [string range $line 0 7] == "00100010" } then {
            puts $sock "[get_data "00100110"][get_data "00100101"][get_data "00100100"][get_data "00100011"][get_data "00100111"][get_data "00101000"][get_data "00101001"][get_data "00101010"][get_data "00101011"][get_data "00101100"][get_data "00101101"][get_data "00000000"]"
         } else {
# Send the vJTAG Commands to Update the LED and SEG
# the data returned are always for the command of the previous call 
	    set led [get_data "00100001"]
	    set seg [get_data "00000000"]
            puts $sock $seg$led
         }
      }
  }
}

#Start that Server
Start_Server $fpga

