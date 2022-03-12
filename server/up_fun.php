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

  // function to print file signature
  function file_signature($fname) {
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
  }
?>
