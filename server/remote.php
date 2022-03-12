<!DOCTYPE HTML>
<html>
<head>
    <style>
      @font-face { font-family: LCD; src: url('led_counter-7.ttf'); }
      .label { font-family: Courier New; color:red; font-weight: bold; }
      pre { font-size:125%; line-height:60%; }
    </style>
  <title>LABARC - remote FPGA board</title>
</head>
<body>
  <h1>LOAC - access to remote FPGA boards</h1>
  <form enctype="multipart/form-data" action="remote.php" method="POST">
    <p>First simulate, then upload Your
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
       and .jpeg., .jpg, .pdf files,
       or a mix of any of those.
       Do not use any sub-directory.</p>
    <p>In order to get credits, You need to put Your name in the first line of the main file.<br>
       You must put a comment marker at the start of that line.</p>
    <p>In the second line, as comment, put the title of Your project.</p>
    <p>In case You upload only one file, this is the one that needs Your name in it.
       Be careful to use the appropriate comment marker for the language the files is written in.
       In case You upload a .zip file, the main file to put Your name in is top.sv, if present.
       If top.sv is not present, the main file is the .c or .s file  one that
       defines <code>main</code>.</p>
    <input type="radio" name="display" value="LCD"> LCD
    <input type="radio" name="display" value="RISC"> RISC-V
    <input type="file" name="uploaded_file" id=browse></input>
    <br style="line-height:180%">
    <input type="submit" value="Upload" id=upload></input>
  </form>
<?PHP
  $subdir = "syn";
  $maxSize = 300000000;
  include 'uploaded.php';

  if(!empty($path)) {
     // Put enough space characters in front of the IP so that even one-digit IPs
     // can be read grabbing the last 18 characters of the file. 
	  file_put_contents($file, "\n/*           " . get_client_ip() . " */\n", FILE_APPEND);
     echo "<div style=\"font-size:75%\" id=\"serverData\">The file "
          . basename( $_FILES['uploaded_file']['name']) .
          " has been uploaded.<br>\n<h4>requesting synthesis</h4></div>\n";
     echo file_get_contents("fpga.html");
     echo "<script type=\"text/javascript\">\n";
     echo "let path = \"". $path ."\";";
     echo "let process = \"synthesis\";";
     echo "let display = \"". $_POST["display"] ."\";";
     echo file_get_contents("remote.js");
     echo "</script>\n";
  }
?>
</body>
</html>

