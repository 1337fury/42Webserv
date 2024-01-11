#!/usr/bin/env node

console.log("Content-type: text/html\r\n\r\n");

// HTML header
console.log(`
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Guessing Game</title>
  <style>
    body {
      font-family: 'Arial', sans-serif;
      text-align: center;
      background-color: #f4f4f4;
      margin: 50px;
    }

    h1 {
      color: #333;
    }

    p {
      color: #555;
      margin-bottom: 20px;
    }

    input {
      padding: 8px;
      font-size: 16px;
    }

    button {
      padding: 10px 20px;
      font-size: 16px;
      background-color: #4caf50;
      color: white;
      border: none;
      cursor: pointer;
    }

    button:hover {
      background-color: #45a049;
    }

    #result {
      font-weight: bold;
      color: #333;
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <h1>Guessing Game</h1>
  <p>Try to guess the number between 1 and 10:</p>
  <input type="number" id="userGuess" min="1" max="10">
  <button onclick="checkGuess()">Submit Guess</button>
  <p id="result"></p>

  <script>
    const targetNumber = Math.floor(Math.random() * 10) + 1;
    let attempts = 0;

    function checkGuess() {
      const userGuess = document.getElementById('userGuess').value;
      attempts++;
	
	  if (attempts > 10) {
		document.getElementById('result').innerHTML = \`<span style="color: red;">You have exceeded the maximum number of attempts. Please refresh the page to try again.</span>\`;
		window.stop();
	  }
      else if (userGuess == targetNumber) {
        document.getElementById('result').innerHTML = \`<span style="color: green;">Congratulations! You guessed the number \${targetNumber} in \${attempts} attempts.</span>\`;
      } else {
        document.getElementById('result').innerHTML = \'<span style="color: red;">Try again. Incorrect guess.</span>\';
		console.log("targetNumber: " + targetNumber);
      }
    }
  </script>
</body>
</html>
`);
