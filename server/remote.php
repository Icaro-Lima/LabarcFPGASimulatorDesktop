<!DOCTYPE HTML>
<html>
<head>
    <style>
      @font-face { font-family: LCD; src: url('led_counter-7.ttf'); }
      .label { font-family: Courier New; color:red; font-weight: bold; }
      pre { font-size:125%; line-height:60%; }
    </style>
  <title>LABARC - FPGA remota</title>
</head>
<body>
  <h1>LOAC - acesso remoto a placas FPGA</h1>
  <form enctype="multipart/form-data" action="remote.php" method="POST">
    <p>Primeiro simule, depois faça upload do seu arquivo
       <a href="top.sv">Systemverilog</a>,&nbsp;
       <a href="inst.101">código de máquina</a>,&nbsp;
       <a href="inst.s">assembly</a>,&nbsp;
       <a href="shift.c"> &nbsp;C&nbsp;</a>, ou&nbsp;
       <a href="riscv1.zip">comprimido</a>.&nbsp;
       O arquivo comprimido pode conter
       somente arquivos SystemVerilog .sv,
       somente um arquivo de código de máquina inst.101,
       somente arquivos .s,
       somente arquivos .c e .h,
       adicionalmente arquivos .jpeg., .jpg, .pdf,
       ou uma mistura de todos estes, mas somente destes.</p>
    <p>Para ganhar centavos, você precisa colocar seu nome na primeira linha do arquivo principal.<br>
       No início da linha, você precisa marcar a mesma como comentário.</p>
    <p>Na segunda linha, em comentário, deve constar o nome do seu projeto.</p>
    <p>Não é necessário informar sua matrícula.
       Não coloque matrícula nem na primeira nem na segunda linha.</p>
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

