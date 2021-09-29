<!DOCTYPE HTML>
<html>
<head>
    <style>
      @font-face { font-family: LCD; src: url('led_counter-7.ttf'); }
      .label { font-family: Courier New; color:red; font-weight: bold; }
      pre { line-height:60%; }
    </style>
  <title>LABARC - remote RISC-V ISA simulation</title>
</head>
<body>
  <h1>LOAC - RISC-V instruction simulator</h1>
  <form enctype="multipart/form-data" action="spike.php" method="POST">
    <p>Get Your
       <a href="inst.101">binary instruction code</a>,&nbsp;
       <a href="inst.s">assembly</a>,&nbsp;
       <a href="shift-pc.c"> &nbsp;C&nbsp;</a>, or&nbsp;
       <a href="lupi-pc.zip">compressed</a>&nbsp; file
       and hit <i>Enter</i> in the input argument field. </p>
      <p>The compressed .zip file may contain
       only assembly .s files,
       only .c and .h C files,
       a binary instruction code file called inst.101,
       or a mix of any of those.
       The .zip file must not contain any directories.</p>
    <input type="file" name="uploaded_file" id=browse></input>
    &nbsp; &nbsp; Input aguments:<input type="text" name="args">
  </form>
<?PHP
  $subdir = "isa";
  $maxSize = 300000;
  include 'uploaded.php';

  if(!empty($path)) {
     // Append submitted info to the file
     // Use comment syntax valid is assembly as well as in C
     // Aything can be appended to a .zip file.
     file_put_contents($file,
                       "\n/* |" . $_POST["args"] . "|"
     // Put enough space characters in front of the IP so that even one-digit IPs
     // can be read grabbing the last 18 characters of the file.
                       . "           " . get_client_ip() . "*/\n",
                       FILE_APPEND);
     echo "<div style=\"font-family:monospace\" id=\"serverData\">The file "
          . basename( $_FILES['uploaded_file']['name']) .
          " has been uploaded.<br>\n<h4>Requesting simulation</h4></div>\n";
     echo file_get_contents("spike.html");
     echo "<script type=\"text/javascript\">\n";
     echo "let path = \"". $path ."\";";
     echo "let process = \"compilação\";";
     echo file_get_contents("spike.js");
     echo "</script>\n";
  }
?>
</body>
</html>

