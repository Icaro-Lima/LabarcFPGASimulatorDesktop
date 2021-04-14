<?php
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
    if( $_FILES['uploaded_file']['size'] > 30000 ) {
       echo "Error: file size ". $_FILES['uploaded_file']['size']
            ." bytes above limit of 30000 bytes";
       $path = false;
    } else {
       $path = trim(shell_exec("mktemp -d -p /home/labarc01/". $subdir ." 2>&1"));
       $file = $path . "/" . basename( $_FILES['uploaded_file']['name']);
       exec("chmod g+rwx ". $path, $o, $chp);
       if( $chp != 0 ) {
           echo "Error changing the directory permissions in ". $path;
           $path = false;
       } else {
          if(!move_uploaded_file($_FILES['uploaded_file']['tmp_name'], $file)) {
             echo "There was an error uploading the file to ". $path .", please try again!";
             $path = false;
          } else {
             exec("chmod g+r ". $file, $o, $chf);
             if( $chp != 0 || $chf != 0 ) {
                 echo "Error changing the file permissions in ". $file;
                 $path = false;
             }
          }
       }
    }
  }
?>

