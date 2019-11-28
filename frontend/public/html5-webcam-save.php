<!-- Webcam code based on Pedro Lobito's answer at https://stackoverflow.com/questions/44009823/how-to-use-the-webcam-to-take-a-picture-and-convert-it-into-an-image-file  -->

<?php
$rawData = $_POST['imgBase64'];
print $rawData;
$filteredData = explode(',', $rawData);
$unencoded = base64_decode($filteredData[1]);
$datime = date("Y-m-d-H.i.s", time() ) ; # - 3600*7
$userid  = $_POST['userid'] ;
// name & save the image file
$fp = fopen('images/'.$datime.'-'.$userid.'.jpg', 'w');
fwrite($fp, $unencoded);
fclose($fp);
