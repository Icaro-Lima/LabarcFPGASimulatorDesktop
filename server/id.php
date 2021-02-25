<!DOCTYPE html>
<html>
<head>
  <title>Upload your key</title>
</head>
<body>
  <form enctype="multipart/form-data" action="id.php" method="POST">
    <p>
      <label for="fname">Nome completo, <b>igualzinho</b> ao Controle Academico:<br></label>
      <input type="text" id="fname" name="fname" size="50"><br>
    </p>
    <p>
      <label for="upload">Por favor faça upload do seu arquivo ~/.ssh/id_rsa.pub :
        <br style="line-height:70%"></label>
      <input type="file" id="upload" name="uploaded_file"></input>
      <br style="line-height:180%">
      <input type="submit" value="Upload"></input>
    </p>
  </form>
<?PHP
  if(isset($_POST['fname']) &&
     !empty($_FILES['uploaded_file']) &&
     !empty($_FILES['uploaded_file']['name']) &&
     !empty($_FILES['uploaded_file']['tmp_name']) )
  {
    $file = $_POST['fname'] . "_" . basename( $_FILES['uploaded_file']['name']);
    if(!move_uploaded_file($_FILES['uploaded_file']['tmp_name'], "uploads/" . $file)) {
          echo "There was an error uploading the file to ". $file .", please try again!";
          print_r($_FILES);
    } else {
          echo "Uploaded to ". $file;
    }
  }
?>
</body>
</html>
