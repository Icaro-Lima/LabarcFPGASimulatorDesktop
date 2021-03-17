<!DOCTYPE HTML>
<html>
<head>
  <title>LABARC- uso remoto</title>
</head>
<body>
  <form enctype="multipart/form-data" action="remote.php" method="POST">
    <p>Primeiro simule, depois faça upload do seu arquivo top.sv</p>
    <p>Para ganhar centavos, você precisa colocar seu nome na primeira linha do top.sv<br>
       No início da linha, você precisa marcar a mesma como comentário.</p>
    <p>Na segunda linha, em comentário, deve constar o nome do exercício ou do circuito.<br>
      Se quiser, pode colocar sua matrícula na segunda linha, mas não na primeira linha.</p>
    <input type="file" name="uploaded_file"></input>
    <br style="line-height:180%">
    <input type="submit" value="Upload"></input>
  </form>
<?PHP
  if(!empty($_FILES['uploaded_file']) &&
     !empty($_FILES['uploaded_file']['name']) &&
     !empty($_FILES['uploaded_file']['tmp_name']) )
  {
    $path = "uploads";
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
             echo "The file ". basename( $_FILES['uploaded_file']['name'])
                  ." has been uploaded.<br>\n";
             echo "
<div id=\"serverData\">Solicitando síntese</div>
<script type=\"text/javascript\">
//check for browser support
if(typeof(EventSource)!==\"undefined\") {
        //create an object, passing it the name and location of the server side script
        var eSource = new EventSource(\"send_sse.php?dir=". $path ."\");
	eSource.onmessage = function(event) {
                var r = event.data.indexOf(\"<h4>Agora digite: ./remote \");
                if( r == -1 ) {
                   //write the received data to the page
                   document.getElementById(\"serverData\").innerHTML = event.data;
		} else {
		   var name_fpga = event.data.substr(r+27).split(\" \");
		   document.getElementById(\"serverData\").innerHTML = name_fpga[0] + name_fpga[1];
                }
	};
} else document.getElementById(\"serverData\").innerHTML=
          \"Whoops! Your browser does not receive server-sent events.\";
</script>
	\n";
             echo file_get_contents("fpga.html");
	     echo "<script src=\"remote.js\"></script>\n";

          }
       }
    }
  }
?>
</body>
</html>

