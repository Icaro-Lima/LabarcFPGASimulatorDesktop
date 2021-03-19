These are the server files that allow using FPGA boards remotely.
They are stored here only for reference.

#### HTTP+NFS+MUTEX+SSH server
```
/var/www/html/hdl
   remote.php
   remote.js
   send_sse.php
   client.php
   fpga.html

/labarc/TOP  (this files get copied to a directory in /home/labarc01/syn)
   DE0_Nano.sv
   DE0_SOC.sv
   Makefile      and also .qpf .qsf .sdc
```
#### NFS clients and JTAG servers where FPGA boards are connected
```
/labarc/util
   vJTAG_interface.sv
   qr.tcl
```
#### NFS mounted /home/labarc01 in server and clients
```
   launch.sh
   launch
   search_copy_TOP
   search_copy_TOP_server
```
### How it works

HTTP server, NFS server, MUTEX server, and SSH server are running on the same computer.
The /home directory is shared via NFS.
The MUTEX server process `search_copy_TOP_server` is running
from the labarc01 account.
The HTTP server has write access to /home/labarc01.

When the user clicks `Upload` in `remote.php`,
the HTTP server creates a new directory in /home/labarc01/syn and
puts the uploaded Systemverilog file `top.sv` into it.
The HTTP server has no FPGA board connected to it.

FPGA boards are connected to several computers which are all NFS clients.
There is a `launch` command running, one for each FPGA board.
The `launch` commands are started by user labarc01 according to `labarc.sh`.
Periodically, the `launch` command tries to connect to the MUTEX server.

The MUTEX server allows only one connection at a time.
The MUTEX server executes the comamnd `search_copy_TOP`, which looks for a
directory in /home/labarc01/syn which has exactly one file in it.
If it finds such a directory, it copies files from `/labarc/TOP` into it
and then returns the directory name to the `launch` command.

When a `launch` command obtains a diretory name, it performs synthesis,
configures its FPGA board, and starts a JTAG server using `qr.tcl`.
Output messages are put into a log file which is periodically being read
by the user's browser using `send_sse.php`.
After starting the JTAG server, an output message is sent informing the computer
name and the FPGA number. The user's browser runs `remote.js` which uses
this information to make requests to the JTAG server via `client.php`.
The return values from these requests animate the graphical interface
of the FPGA board in `fpga.html`.

Closing the browser tab of window os clicking on the `Browse...` or `Upload` button
sends an exit message to the JTAG server. This makes the FPGA available for
another run.
If no such exit message is received, the JTAG server is killed after 5 minutes.

