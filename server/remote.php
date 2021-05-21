<!DOCTYPE HTML>
<html>
<head>
    <style>
      @font-face { font-family: LCD; src: url('led_counter-7.ttf'); }
      .label { font-family: Courier New; color:red; font-weight: bold; }
      pre { font-size:125%; line-height:60%; }
    </style>
  <title>LABARC- uso remoto</title>
</head>
<body>
  <form enctype="multipart/form-data" action="remote.php" method="POST">
    <p>Primeiro simule, depois faça upload do seu arquivo top.sv ou top.zip</p>
    <p>Para ganhar centavos, você precisa colocar seu nome na primeira linha do top.sv<br>
       No início da linha, você precisa marcar a mesma como comentário.</p>
    <p>Na segunda linha, em comentário, deve constar o nome do exercício ou do circuito.</p>
    <p>Se quiser, pode colocar sua matrícula na terceira linha,
        mas não a coloque nem na primeira nem na segunda linha.</p>
    <input type="file" name="uploaded_file" id=browse></input>
    <input type="radio" name="display" value="LCD"> LCD
    <input type="radio" name="display" value="RISC"> RISC-V
    <br style="line-height:180%">
    <input type="submit" value="Upload" id=upload></input>
  </form>
<?PHP
  $subdir = "syn";
  include 'uploaded.php';

  if(!empty($path)) {
     // Put enough space characters in front of the IP so that even one-digit IPs
     // can be read grabbing the last 18 characters of the file. 
	  file_put_contents($file, "\n/*           " . get_client_ip() . " */\n", FILE_APPEND);
     echo "<div style=\"font-size:75%\" id=\"serverData\">The file "
          . basename( $_FILES['uploaded_file']['name']) .
          " has been uploaded.<br>\n<h4>solicitando síntese</h4></div>\n";
     echo file_get_contents("fpga.html");
     echo "<script type=\"text/javascript\">\n";
     echo "let path = \"". $path ."\";";
     echo "let process = \"síntese\";";
     echo "let display = \"". $_POST["display"] ."\";";
     echo file_get_contents("remote.js");
     echo "</script>\n";
  }
?>
</body>
</html>

