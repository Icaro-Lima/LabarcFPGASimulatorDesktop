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

  if( isset($_GET["name"]) && isset($_GET["port"]) && isset($_GET["data"]) &&
      !empty($_GET["name"]) && !empty($_GET["data"]) ) {
    $host    = $_GET["name"];
    $port    = $_GET["port"];
    $message = substr($_GET["data"],0,8) . " " . get_client_ip() . "\n";
    // Error messages must start with letter 'S'
    // create socket
    $socket = socket_create(AF_INET, SOCK_STREAM, 0) or die("Socket creation failed\n");
    // connect to server
    $result = socket_connect($socket, $host, $port) or die("Server connection failed\n");
    // send string to server
    socket_write($socket, $message, strlen($message)) or die("Sending data failed\n");
    // get server response
    if( substr_compare($message,"0011",0,4) == 0 || substr_compare( $message,"0000",0,4) == 0 ) {
       $nr = 32;
    } else if( substr_compare( $message,"00100011",0,8) == 0 ) {
       $nr = 24;
    } else {
       $nr = 4;
    }
    $result = socket_read ($socket, $nr) or die("Server response failed\n");
    echo $result;
    // close socket
    socket_close($socket);
  }
?>
