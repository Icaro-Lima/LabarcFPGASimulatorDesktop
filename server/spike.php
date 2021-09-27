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
  <h1>LOAC - simulador de instruções RISC-V</h1>
  <form enctype="multipart/form-data" action="spike.php" method="POST">
    <p>Procure seu arquivo
       <a href="inst.101">código de máquina</a>,&nbsp;
       <a href="inst.s">assembly</a>,&nbsp;  
       <a href="shift.c"> &nbsp;C </a>,&nbsp; ou
       um arquivo <a href="lupi-pc.zip">comprimido</a>
       e dê <i>Enter</i> no campo de valores de entrada. </p>
    <p> O arquivo comprimido pode conter arquivos em C (*.c), arquivos Assembly (*.s),
        ou uma mistura destes. Não pode ter diretório dentro do arquivo comprimido. </p>
    <input type="file" name="uploaded_file" id=browse></input>
    &nbsp; &nbsp; Valores de entrada:<input type="text" name="args">
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

