#!/usr/bin/env node

console.log('Content-Type: text/html');
console.log('');

const envVars = process.env;

const style = `
	<style>
		body {
			font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
			display: flex;
			flex-direction: column;
			justify-content: center;
			align-items: center;
			height: 100vh;
			background-color: #f4f4f4;
		}
		table {
			border-collapse: collapse;
			width: 60%;
			background-color: #fff;
		}
		th, td {
			text-align: left;
			padding: 12px;
			border-bottom: 1px solid #ddd;
		}
		th {
			background-color: #6c7ae0;
			color: white;
		}
		tr:nth-child(even) {
			background-color: #f2f2f2;
		}
		.artwork {
			margin-top: 20px;
			text-align: center;
		}
	</style>
`;

const createTable = (envVars) => {
  let rows = '';
  for (const [key, value] of Object.entries(envVars)) {
    rows += `<tr><td>${key}</td><td>${value}</td></tr>`;
  }
  return `<table>${rows}</table>`;
};

console.log('<html>');
console.log('<head>');
console.log('<title>Environment Variables</title>');
console.log(style);
console.log('</head>');
console.log('<body>');
console.log('<h1 style="color: #ff5722;">Environment Variables</h1>');
console.log(createTable(envVars));
console.log('</body>');
console.log('</html>');