<?php include('configuration.php') ?>
<html>
<h2> Mock Film Management Component: Display one film </h2>
<?php
$film_id = $_GET['film_id'];

$sql_stmt = <<<__QUERY
Select	title, rental_rate 
from 	film 
where	film_id = ?;
__QUERY;

if($stmt = $conn->prepare($sql_stmt))
{
	$stmt->bind_param("s", $film_id);
	$stmt->execute();
	$stmt->bind_result($title, $rental_rate);
	$stmt->store_result();
	if($stmt->num_rows > 0)
	{
		while($stmt->fetch())
		{
			echo "<b>Film: ".$title."</b><br><br>";
			echo "<b>Rental Rate: $".$rental_rate."</b><br><br>";
		}
		$stmt->free_result();
	}
	else
	{
		echo "No results found. <br>";
	}
}
else
{
	echo "Error in creating prepared statement.<br>";
}

$sql_stmt = <<<__QUERY
select	count(*) 
from 	inventory 
where 	film_id = ?;
__QUERY;

if($stmt = $conn->prepare($sql_stmt))
{
	$stmt->bind_param("s", $film_id);
	$stmt->execute();
	$stmt->bind_result($inventory);
	$stmt->store_result();
	if($stmt->num_rows > 0)
	{
		while($stmt->fetch())
		{
			echo "<b>Number of copies in the inventory: ".$inventory.".</b><br>";
		}
		$stmt->free_result();
	}
	else
	{
		echo "No results found. <br>";
	}
}
else
{
	echo "Error in creating prepared statement.<br>";
}

$sql_stmt = <<<__QUERY
select	count(*) 
from 	rental 
where 	inventory_id in (select	inventory_id 
						 from	inventory 
						 where film_id = ?);
__QUERY;


if($stmt = $conn->prepare($sql_stmt))
{
	$stmt->bind_param("s", $film_id);
	$stmt->execute();
	$stmt->bind_result($rental);
	$stmt->store_result();
	if($stmt->num_rows > 0)
	{
		while($stmt->fetch())
		{
			echo "<b>Number of times rented: ".$rental.".</b><br>";
		}
		$stmt->free_result();
	}
	else
	{
		echo "No results found. <br>";
	}	
}
else
{
	echo "Error in creating prepared statement.<br>";
}

$sql_stmt = <<<__QUERY
select	sum(amount) 
from 	payment 
where 	rental_id in (select	rental_id 
					  from 		rental 
					  where 	inventory_id in (select	inventory_id 
												 from	inventory 
												 where 	film_id = ?));
__QUERY;
if($stmt = $conn->prepare($sql_stmt))
{
	$stmt->bind_param("s", $film_id);
	$stmt->execute();
	$stmt->bind_result($fees);
		$stmt->store_result();
	if($stmt->num_rows > 0)
	{
		while($stmt->fetch())
		{
			echo "<b>Rental fees collected: $".$fees.".</b><br>";
		}
		$stmt->free_result();
	}
	else
	{
		echo "No results found. <br>";
	}	
}
else
{
	echo "Error in creating prepared statement.<br>";
}
?>
</html>
