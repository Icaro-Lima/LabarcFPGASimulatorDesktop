<?PHP
  include 'ip_fun.php';

  // function to print file signature
  function file_signature($fname) {
     exec("chmod g+rw ". $fname ." ; ".
          "echo Certificado de entrega > ". $fname .".sign; ".
          "ls -l -g -o ". $fname ." | cut -d' ' -f3 >> ". $fname .".sign ; ".
          "md5sum ". $fname ." | cut -d' ' -f1 >> ". $fname .".sign ; ".
          "echo >> ". $fname ." ; ".
          "echo /\* \  \  \  \  \  ". get_client_ip() ." \*/ >> ". $fname ." ; ".
          "ls -g -o --full-time ". $fname ." | cut -d' ' -f4-5 >> ". $fname .".sign ; ".
          "gpg --homedir ../../.gnupg --clearsign ". $fname .".sign", $o, $r);
     // First record the checksum of the original uploaded file,
     // then append the IP to it,
     // and only then, as the last operation, record the time stamp.
     // In this way, students can verify the checksum according to the
     // uploaded file in their posession, and the professor can search for
     // a timestamp on the server that matches exactly.
     // Put enough space characters in front of  get_client_ip()  so that even one-digit IPs
     // can be read grabbing the last 18 characters of the file. 
     $o = file($fname .".sign.asc");
     echo "<code style=\"display:inline-block;font-size:25%;line-height:8px\">";
     foreach($o as $item){
         echo substr($item,0,-1) . "<br>\n";
     }
     echo "</code>";
     exec("rm ". $fname .".sign ". $fname .".sign.asc", $o, $r);
  }
?>
