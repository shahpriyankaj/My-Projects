<?php
$server = "dcm.uhcl.edu";
$username = "shahp";
$password = "1458385";
$database = "sakila";

$films_path = "/c533315fa02shahp/films.php";

$film_path = "/c533315fa02shahp/film.php";


$conn = new mysqli($server, $username, $password, $database);

if ($conn->connect_error) 
{
	die("Connection failed: " . $conn->connect_error);
}
?>