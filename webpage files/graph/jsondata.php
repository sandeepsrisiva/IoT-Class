<?php
  session_start();
  $var=$_GET['r'];
  
  
  include('config.php');


//execute query
 $query="select createdat,".$var." from wms order by id DESC LIMIT 0,10";
        $result=mysqli_query($con,$query);
        $rowcount=mysqli_num_rows($result);

$data = array();
while($row = mysqli_fetch_array($result)) {

	$data[] = $row;
}
$tempdata = array();
foreach ($data as $key => $row)
{
    $tempdata[$key] = $row['createdat'];
}
array_multisort($tempdata, SORT_ASC, $data);

print json_encode($data);
?>
