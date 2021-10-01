<!DOCTYPE HTML>
<html>
<head>
    <style>
      pre { font-size:125%; line-height:60%; }
    </style>
  <title>LABARC - Entrega Falstad</title>
</head>
<body>
  <form enctype="multipart/form-data" action="index.php" method="POST">
    <p>No Falstad, escolhe File &#8594; Export as text...</p>
    <p>Clique no centro da janela que aparece e dê Ctrl-A Ctrl-C,
       depois volte aqui, clique em cima de "Enter text" e dê Ctrl-V.</p>
    <p>Acrescente uma linha inciando com / e escreve seu nome nela.</p>
    <p>Acrescente mais uma linha inciando com / e escreve um título descritivo para o circuito.</p>
    <p>Agora clique em "Upload".</p>
    <textarea rows="5" cols="60" name="code" placeholder="Enter text"></textarea>
    <br style="line-height:180%">
    <input type="submit" value="Upload" id=upload></input>
  </form>
<?PHP
  if(!empty($_POST["code"])) {
     $fname = tempnam("/home/labarc01/falstad", "f");
     $f = fopen($fname, "w") or die("Unable to open file!");
     fwrite($f, $_POST["code"]);
     fclose($f);
     exec("chmod g+rw ". $fname, $o, $chp);
     echo("Circuito entregue");
  }
?>
</body>
</html>
