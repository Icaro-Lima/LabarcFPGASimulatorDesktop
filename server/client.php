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

  define("MAX_LINE_LENGTH", 100);

  if( isset($_GET["name"]) && isset($_GET["port"]) && isset($_GET["data"]) &&
      !empty($_GET["name"]) && $_GET["port"] > 1000 &&
      ( !preg_match('/[^01]/', $_GET["data"]) || !strcmp($_GET["data"], "exit") 
        || !preg_match('/[^fhmpqrsuvw]/', $_GET["data"][0]) 
      ) ) {
        // data must contain only 0 and 1  or "exit"
    $host    = $_GET["name"];
    $port    = $_GET["port"];
    $message = $_GET["data"] . " " . get_client_ip() . "\n";
    // Error messages must start with letter 'S'
    // create socket
    $socket = socket_create(AF_INET, SOCK_STREAM, 0) or die("Socket creation failed\n");
    // connect to server
    $result = socket_connect($socket, $host, $port) or die("Server connection failed\n");
    // send string to server
    socket_write($socket, $message, strlen($message)) or die("Sending data failed\n");
    // get server response
    if($result = socket_read ($socket, MAX_LINE_LENGTH, PHP_NORMAL_READ)) {
       echo $result;
    } else {
       echo "\n"; // allow for no response from socket
    }
    // accept more lines if m is set - else remote simulation exits (why?)
    while(isset($_GET["m"]) && ($result = socket_read ($socket, MAX_LINE_LENGTH, PHP_NORMAL_READ))) {
       echo "\n<br>" . preg_replace('/ /', '&nbsp;',
                       preg_replace('/>/', '&#62',
                       preg_replace('/</', '&#60', $result)));
    }
    // close socket
    socket_close($socket);
  }
?>
