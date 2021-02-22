These are the server files that allow using FPGA board remotely.
They are stored here only for reference.

```
lad:/var/www/html/hdl  (HTTP server)
   sintese.php
   send_sse.php

lad:/labarc/TOP  (this directory gets copied to user server home)
   DE0_Nano.sv
   Makefile

/labarc/util  (computer connected to FPGA board)
   vJTAG_interface.sv
   qr.tcl

~labarc01   (user server home)
   launch.sh
   launch
   search_copy_TOP
   search_copy_TOP_server
```

The file launch.sh has the commands needed to run launch,
which runs on the computer connected to FPGA board
The same procedure is used for search_copy_TOP_server,
however, this command runs on the NFS server (in our case same as HTTP server)

