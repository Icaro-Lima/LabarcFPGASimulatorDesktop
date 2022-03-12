<?php
  include 'up_fun.php';

  if(!empty($_FILES['uploaded_file']) &&
     !empty($_FILES['uploaded_file']['name']) &&
     !empty($_FILES['uploaded_file']['tmp_name']) )
  {
    if( $_FILES['uploaded_file']['size'] > $maxSize ) {
       echo "Error: file size ". $_FILES['uploaded_file']['size']/1000000
            ." MB above limit of ". $maxSize/1000000 ." MB";
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
             if( $chf != 0 ) {
                 echo "Error changing the file permissions in ". $file;
                 $path = false;
             }
             file_signature($file);
          }
       }
    }
  }
?>
