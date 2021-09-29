<!DOCTYPE HTML>
<html>
<head>
    <style>
      @font-face { font-family: LCD; src: url('led_counter-7.ttf'); }
      .label { font-family: Courier New; color:red; font-weight: bold; }
      pre { font-size:125%; line-height:60%; }
    </style>
  <title>LABARC- remote simulation</title>
</head>
<body>
  <h1>LOAC - FPGA board simulator</h1>
  <form enctype="multipart/form-data" action="simulate.php" method="POST">
    <p>Simulating FPGA is not worth any credit, put compilation for simulation
       is much faster than logic synthesis for the real circuit.</p>
    <p>Upload Your
       <a href="top.sv">Systemverilog</a>,&nbsp;
       <a href="inst.101">binary instruction code</a>,&nbsp;
       <a href="inst.s">assembly</a>,&nbsp;
       <a href="shift-fpga.c"> &nbsp;C&nbsp;</a>, or&nbsp;
       <a href="riscv1.zip">compressed</a>&nbsp; file.
       The compressed .zip file may contain
       only SystemVerilog .sv files,
       only assembly .s files,
       only .c and .h C files,
       a binary instruction code file called inst.101,
       or a mix of any of those.</p>
    <input type="radio" name="display" value="LCD"> LCD
    <input type="radio" name="display" value="RISC"> RISC-V
    <input type="file" name="uploaded_file" id=browse></input>
    <br style="line-height:180%">
    <input type="submit" value="Upload" id=upload></input>
  </form>
<?PHP
  $subdir = "sim";
  $maxSize = 300000;
  include 'uploaded.php';

  if(!empty($path)) {
     // Put enough space characters in front of the IP so that even one-digit IPs
     // can be read grabbing the last 18 characters of the file. 
     file_put_contents($file, "\n/*           " . get_client_ip() . " */\n", FILE_APPEND);
     echo "<div style=\"font-size:75%\" id=\"serverData\">The file "
          . basename( $_FILES['uploaded_file']['name']) .
          " has been uploaded.<br>\n<h4>Solicitando simulação</h4></div>\n";
     echo file_get_contents("fpga.html");
     echo "<script type=\"text/javascript\">\n";
     echo "let path = \"". $path ."\";";
     echo "let process = \"compilação\";";
     echo "let display = \"". $_POST["display"] ."\";";
     echo file_get_contents("remote.js");
     echo "</script>\n";
  }
?>
</body>
</html>

