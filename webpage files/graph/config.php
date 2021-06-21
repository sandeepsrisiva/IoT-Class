<?php

	$servername = "localhost";
	$username = "root";
	$password = "Test@123";
	$dbname = "iotdata";

	$con=mysqli_connect($servername,$username,$password,$dbname);

	if ($con->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	}
            
?>