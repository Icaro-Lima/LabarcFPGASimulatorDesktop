<!DOCTYPE HTML>
<html>
<head>
    <style>
      @font-face { font-family: LCD; src: url('led_counter-7.ttf'); }
      .label { font-family: Courier New; color:red; font-weight: bold; }
    </style>
  <title>LABARC - simulação ISA remota</title>
</head>
<body>
  <form enctype="multipart/form-data" action="spike.php" method="POST">
    <p>Upload Your assembly file .s or C file .c, or compressed file .zip</p>
    <input type="file" name="uploaded_file" id=browse></input>
    <br style="line-height:180%">
    <input type="submit" value="Upload" id=upload></input>
  </form>
<?PHP
  $subdir = "isa";
  include 'uploaded.php';

  if(!empty($path)) {
     // Put enough space characters in front of the IP so that even one-digit IPs
     // can be read grabbing the last 18 characters of the file. 
     file_put_contents($file, "\n/*           " . get_client_ip() . "*/\n", FILE_APPEND);
     echo "<div style=\"font-size:75%\" id=\"serverData\">The file "
          . basename( $_FILES['uploaded_file']['name']) .
          " has been uploaded.<br>\n<h4>Solicitando simulação</h4></div>\n";
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

