<?php include('configuration.php') ?>
<html>
<h2> Mock Film Management Component: Films of title chars
<?php

$select_title = $_GET['select_title'];
echo $select_title.".</h2><br>";
$sql_stmt = <<<__QUERY
Select	count(film_id) 
from	film 
where	title like ?;
__QUERY;

if($stmt = $conn->prepare($sql_stmt))
{
	$stmt->bind_param("s", $select_title);
	$select_title = $select_title."%";
	$stmt->execute();
	$stmt->bind_result($total_films);
	$stmt->store_result();
	if($stmt->num_rows > 0)
	{
		while($stmt->fetch())
		{
			echo "<b>Number of Films for ".$_GET['select_title'].":".$total_films.":"."</b><br>";
		}
		$stmt->free_result();
	}
	else
	{
		echo "No results found.<br>";
	}
}
else
{
	echo "Error in creating prepared statement.<br>";
}

$sql_stmt = <<<__QUERY
Select	film_id, title, length from	film 
where	title like ?;
__QUERY;
if($stmt = $conn->prepare($sql_stmt))
{
	$stmt->bind_param("s", $select_title);
	$select_title = $select_title."%";
	$stmt->execute();
	$stmt->bind_result($film_id, $title, $length);
	$count = 0;
	?>
	<?php
	$stmt->store_result();
	if($stmt->num_rows > 0)
	{
		while($stmt->fetch())
		{
			$count = $count + 1;
			echo $count.". ";
			echo "<a href = '$film_path?film_id=$film_id'>".$title."</a>";	
			echo ": ".$length." minutes.<br>";
		}
		$stmt->free_result();
	}
	else
	{
		echo "No results found.<br>";
	}
}
else
{
	echo "Error in creating prepared statement.<br>";
}
$conn->close();
?>
</html>