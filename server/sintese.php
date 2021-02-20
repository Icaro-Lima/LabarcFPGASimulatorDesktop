<!DOCTYPE html>
<html>
<head>
  <title>Upload your files</title>
</head>
<body>
  <form enctype="multipart/form-data" action="sintese.php" method="POST">
    <p>First simulate, then upload Your top.sv</p>
    <input type="file" name="uploaded_file"></input><br />
    <br>
    <input type="submit" value="Upload"></input>
  </form>
<?PHP
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
                //write the received data to the page
                document.getElementById(\"serverData\").innerHTML = event.data;
        };
} else document.getElementById(\"serverData\").innerHTML=
          \"Whoops! Your browser does not receive server-sent events.\";
</script>
               \n";
          }
       }
    }
  }
?>
</body>
</html>
