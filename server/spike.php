<!DOCTYPE HTML>
<html>
<head>
    <style>
      @font-face { font-family: LCD; src: url('led_counter-7.ttf'); }
      .label { font-family: Courier New; color:red; font-weight: bold; }
      pre { line-height:60%; }
    </style>
  <title>LABARC - simulação ISA remota</title>
</head>
<body>
  <form enctype="multipart/form-data" action="spike.php" method="POST">
    <p>Procure seu arquivo assembly .s ou C .c, ou um arquivo comprimido .zip
       e dê <i>Enter</i> no campo de valores de entrada. </p>
    <input type="file" name="uploaded_file" id=browse></input>
    &nbsp; &nbsp; Valores de entrada:<input type="text" name="args">
  </form>
<?PHP
  $subdir = "isa";
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

