<!DOCTYPE HTML>
<html>
<head>
    <style>
      pre { font-size:125%; line-height:60%; }
    </style>
  <title>LABARC - Entrega Falstad</title>
</head>
<body>
  <form enctype="multipart/form-data" action="index.php" method="POST">
    <p>No Falstad, escolhe File &#8594; Export as text...</p>
    <p>Clique no centro da janela que aparece e dê Ctrl-A Ctrl-C,
       depois volte aqui, clique em cima de "Enter text" e dê Ctrl-V.</p>
    <p>Acrescente uma linha inciando com / e escreve seu nome nela.</p>
    <p>Acrescente mais uma linha inciando com / e escreve um título descritivo para o circuito.</p>
    <p>Agora clique em "Upload".</p>
    <textarea rows="5" cols="60" name="code" placeholder="Enter text"></textarea>
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

  if(!empty($_POST["code"])) {
     $fdir = "/home/labarc01/falstad";
     $fname = tempnam($fdir, "f");
     $f = fopen($fname, "w") or die("Unable to open file!");
     fwrite($f, $_POST["code"]);
     fclose($f);
     exec("chmod g+rw ". $fname ." ; ".
          "echo Certificado de entrega > ". $fname .".sign; ".
          "ls -g -o --full-time ". $fname ." | cut -d' ' -f3-5 >> ". $fname .".sign ; ".
          "md5sum ". $fname ." | cut -d' ' -f1 >> ". $fname .".sign ; ".
          "gpg --homedir ../../.gnupg --clearsign ". $fname .".sign", $o, $r);
     $o = file($fname .".sign.asc");
     echo "<code style=\"display:inline-block;font-size:25%;line-height:8px\">";
     foreach($o as $item){
         echo substr($item,0,-1) . "<br>\n";
     }
     echo "</code>";
     exec("rm ". $fname .".sign ". $fname .".sign.asc", $o, $r);
     exec("echo \"/\n/". get_client_ip() ."\" >> ". $fname, $o, $r);
  }
?>
</body>
</html>
