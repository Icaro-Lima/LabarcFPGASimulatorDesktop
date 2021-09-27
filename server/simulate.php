<!DOCTYPE HTML>
<html>
<head>
    <style>
      @font-face { font-family: LCD; src: url('led_counter-7.ttf'); }
      .label { font-family: Courier New; color:red; font-weight: bold; }
      pre { font-size:125%; line-height:60%; }
    </style>
  <title>LABARC- simulação remota</title>
</head>
<body>
  <h1>LOAC - simulador de placa FPGA</h1>
  <form enctype="multipart/form-data" action="simulate.php" method="POST">
    <p>Simular FPGA não vale centavos, mas compilação para simulação
       é bem mais rápida do que a síntese do circuito real.</p>
    <p>Por favor, faça upload do seu arquivo
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
       ou uma mistura de todos estes.</p>
    <input type="file" name="uploaded_file" id=browse></input>
    <input type="radio" name="display" value="LCD"> LCD
    <input type="radio" name="display" value="RISC"> RISC-V
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

