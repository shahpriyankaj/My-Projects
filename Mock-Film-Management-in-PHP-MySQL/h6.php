<?php include('configuration.php') ?>
<html>
<h2>Mock Film Management Component</h2><br>
<h3>Please select the first character of the film titles:</h3>
<br>
<?php
$sql_stmt = <<<__QUERY
Select	distinct substring(title,1,1) as t 
from 	film;
__QUERY;

$result = $conn->query($sql_stmt);

if ($result->num_rows > 0) 
{
	echo "<form action = $films_path method = GET>";
	?>
	<b>Select: </b><select name = "select_title"> 
	<?php
	while($row = $result->fetch_assoc()) 
	{
		echo "<option>".$row[t]."</option>";
	}
	?>	
	</select>
	<input type="submit" value="Submit" onchange = "this.form.submit()">
	</form>
	<?php
} 
else 
{
	echo "No results found!";
}

$conn->close();
?>
</html>