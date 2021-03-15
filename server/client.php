<?php
  if( isset($_GET["name"]) && isset($_GET["fpga"]) && isset($_GET["data"]) &&
      !empty($_GET["name"]) && !empty($_GET["data"]) ) {
    $host    = $_GET["name"];
    $port    = $_GET["fpga"] + 2540;
    $message = $_GET["data"] . "\n";
    // create socket
    $socket = socket_create(AF_INET, SOCK_STREAM, 0) or die("Could not create socket\n");
    // connect to server
    $result = socket_connect($socket, $host, $port) or die("Could not connect to server\n");  
    // send string to server
    socket_write($socket, $message, strlen($message)) or die("Could not send data to server\n");
    // get server response
    $result = socket_read ($socket, 4) or die("Could not read server response\n");
    echo $result;
    // close socket
    socket_close($socket);
  }
?>
