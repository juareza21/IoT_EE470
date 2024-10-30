<!DOCTYPE html>
<html>
    <head>
        <body>
            <h1 style="color:green;"> IoT Course</h1>

<h4> How to call PHP function on the click of a button ?</h4>

<form method="post" action="<?php echo $_SERVER['PHP_SELF'];?>">
	Click to turn ON: <input type="submit" name="fname" value="on">
	<p> </p>
	Click to Turn OFF: <input type="submit" name="fname" value="off">
</form>
<!-- Using PHP Script: passing fname via POST_METHOD-->
<?php
	$var1 = " ";
	  if ($_SERVER["REQUEST_METHOD"] == "POST") {
		   $var1= $_POST["fname"];
		  if (empty($var1)) {
		    $var1= " ";
		} else {
			$var1= $_POST["fname"];
			echo "<p> Last time your click was: $var1 </p>";
			echo "Check the file https://juareza.net/dashboard/results.txt";
		}
	}
	
	$myfile = fopen("results.txt", "w") or die("Unable to open file!");
	fwrite($myfile, $var1);
	fclose($myfile);
            ?>
        </body>
    </head>
</html>
