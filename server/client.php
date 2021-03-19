<?php
  if( isset($_GET["name"]) && isset($_GET["fpga"]) && isset($_GET["data"]) &&
      !empty($_GET["name"]) && !empty($_GET["data"]) ) {
    $host    = $_GET["name"];
    $port    = $_GET["fpga"] + 2540;
    $message = $_GET["data"] . "\n";
    // Error messages must start with letter 'S'
    // create socket
    $socket = socket_create(AF_INET, SOCK_STREAM, 0) or die("Socket creation failed\n");
    // connect to server
    $result = socket_connect($socket, $host, $port) or die("Server connection failed\n");  
    // send string to server
    socket_write($socket, $message, strlen($message)) or die("Sending data failed\n");
    // get server response
    $result = socket_read ($socket, 4) or die("Server response failed\n");
    echo $result;
    // close socket
    socket_close($socket);
  }
?>
