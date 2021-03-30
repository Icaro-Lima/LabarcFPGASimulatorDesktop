<!DOCTYPE HTML>
<html>
<head>
    <style>
      @font-face { font-family: LCD; src: url('led_counter-7.ttf'); }
      .label { font-family: Courier New; color:red; font-weight: bold; }
    </style>
  <title>LABARC- uso remoto</title>
</head>
<body>
  <form enctype="multipart/form-data" action="remote.php" method="POST">
    <p>Primeiro simule, depois faça upload do seu arquivo top.sv</p>
    <p>Para ganhar centavos, você precisa colocar seu nome na primeira linha do top.sv<br>
       No início da linha, você precisa marcar a mesma como comentário.</p>
    <p>Na segunda linha, em comentário, deve constar o nome do exercício ou do circuito.<br>
      Se quiser, pode colocar sua matrícula na segunda linha, mas não na primeira linha.</p>
    <input type="file" name="uploaded_file" id=browse></input>
    <input type="radio" name="display" value="LCD"> LCD
    <input type="radio" name="display" value="RISC"> RISC-V
    <br style="line-height:180%">
    <input type="submit" value="Upload" id=upload></input>
  </form>
<?PHP
  // Function to get the client IP address
  function get_client_ip() {
    $ipaddress = '';
    if (getenv('HTTP_CLIENT_IP'))
        $ipaddress = getenv('HTTP_CLIENT_IP');
    else if(getenv('HTTP_X_FORWARDED_FOR'))
        $ipaddress = getenv('HTTP_X_FORWARDED_FOR');
    else if(getenv('HTTP_X_FORWARDED'))
        $ipaddress = getenv('HTTP_X_FORWARDED');
    else if(getenv('HTTP_FORWARDED_FOR'))
        $ipaddress = getenv('HTTP_FORWARDED_FOR');
    else if(getenv('HTTP_FORWARDED'))
       $ipaddress = getenv('HTTP_FORWARDED');
    else if(getenv('REMOTE_ADDR'))
        $ipaddress = getenv('REMOTE_ADDR');
    else
        $ipaddress = 'UNKNOWN';
    return $ipaddress;
  }

  if(!empty($_FILES['uploaded_file']) &&
     !empty($_FILES['uploaded_file']['name']) &&
     !empty($_FILES['uploaded_file']['tmp_name']) )
  {
    $path = trim(shell_exec("mktemp -d -p /home/labarc01/syn 2>&1"));
    $file = $path . "/" . basename( $_FILES['uploaded_file']['name']);
    exec("chmod g+rwx ". $path, $o, $chp);
    if( $chp != 0 ) {
           echo "Error changing the directory permissions in ". $path;
    } else {
       if(!move_uploaded_file($_FILES['uploaded_file']['tmp_name'], $file)) {
          echo "There was an error uploading the file to ". $path .", please try again!";
       } else {
          exec("chmod g+r ". $file, $o, $chf);
          if( $chp != 0 || $chf != 0 ) {
              echo "Error changing the file permissions in ". $file;
	  } else {
             // Put enough space befire the IP so that even one-digit IPs
             // can be read grabbing the last 18 characters of the file. 
             file_put_contents($file, "\n//           " . get_client_ip() . "\n", FILE_APPEND);
	     echo "<div id=\"serverData\">The file "
		  . basename( $_FILES['uploaded_file']['name']) .
                  " has been uploaded.<br>\n<h4>Solicitando síntese</h4></div>\n";
             echo file_get_contents("fpga.html");
	     echo "<script type=\"text/javascript\">\n";
	     echo "let path = \"". $path ."\";";
	     echo "let display = \"". $_POST["display"] ."\";";
             echo file_get_contents("remote.js");
	     echo "</script>\n";
          }
       }
    }
  }
?>
</body>
</html>

