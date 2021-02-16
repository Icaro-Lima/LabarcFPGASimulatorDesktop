These are the server files that allow using FPGA board remotely.
They are stored here only for reference.

```
/var/www/html/hdl  (HTTP server)
   sintese.php
   send_sse.php

~labarc01   (user server home)
   launch.sh
   launch
   cond_copy_TOP

/labarc/TOP  (computer connected to FPGA board - this directory gets copied to user server home)
   DE0_Nano.sv
   Makefile

/labarc/util  (computer connected to FPGA board)
   vJTAG_interface.sv
   qr.tcl
```

