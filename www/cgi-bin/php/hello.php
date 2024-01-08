<!DOCTYPE html>
<html>
<head>
    <title>PHP Form Example</title>
</head>
<body>
	<?php
		// if the request method is GET, then display hello + name from the query string by splitting the query string at the = sign
		if ($_SERVER["REQUEST_METHOD"] == "GET") {
			// Parse the entire query string
			parse_str($_SERVER["QUERY_STRING"], $queryArray);
			
			// Check if 'name' key exists in the parsed query array
			if (isset($queryArray['name'])) {
				$name = $queryArray['name'];
				echo "<h1>Hello, $name!</h1>";
			} else {
				// If the 'name' key doesn't exist, handle it as you prefer
				echo "<h1>Name parameter is missing.</h1>";
			}
		}
	?>

	<?php
		// Check if there are arguments passed to the script
		if ($argc > 1) {
			// Loop through each element in the $argv array and print it
			foreach ($argv as $key => $value) {
				echo "Argument $key: $value\n";
			}
		} else {
			echo "No arguments provided.\n";
		}
	?>

</body>
</html>