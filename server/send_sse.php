<?php
header('Content-Type: text/event-stream');
header('Cache-Control: no-cache');
if( isset($_GET["dir"]) ) 
{
   $path = strtr($_GET['dir'],"<>?","___");
   $log = $path . "/qq.log";
   $a = "data: <h3>espere a " . $_GET['proc'] . " iniciar</h3>\n";
   if(file_exists($log) && ($handle = fopen("$log", "r")) ) {
         $a = "data: <h3>" . $_GET['proc'] . " iniciada, espere resultado:</h3>\n";
         while($line = fgets($handle)) {
            $a = $a . "data: ". trim($line) ."<br>\n";
         }
   }
} else {
   $a = "data: <br>falha - " . $_GET['proc'] . " não iniciou\n";
}
if( strpos($a, "______________pronto________________") ) {
  echo "retry: 100000\n". $a ."data: <br>\n\n";
  ob_flush();
} else {
  //echo the new number
  echo "retry: 1000\n". $a ."data: <br>\n\n";
  ob_flush();
}
?>

