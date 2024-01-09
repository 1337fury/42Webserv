# import sys
# print("Content-Type: text/plain\r\n\r\n")
# for line in sys.stdin:
# 	print(line)
# sys.stdout.flush()
# sys.stderr.flush()
# sys.exit(0)


#!/usr/bin/python
import sys
import cgi
import cgitb; cgitb.enable()  # Optional; for debugging purposes

# Set color codes
color_input1 = '36'  # Cyan
color_input2 = '35'  # Magenta

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
input1 = form.getvalue('input1')
input2 = form.getvalue('input2')

# Content type for HTML response
print("Content-Type: text/html\r\n")

# HTML header
print("<html>")
print("<head>")
print("<title>Colorful Output</title>")
print("</head>")
print("<body style='background-color:#f0f0f0;'>")

# ASCII art
print("<pre>")
print("  _______ _                 _          _____ _ _            ")
print(" |__   __| |               | |   /\\   / ____(_) |           ")
print("    | |  | |__   __ _ _ __ | |  /  \\ | |     _| |_ ___  ___ ")
print("    | |  | '_ \\ / _` | '_ \\| | / /\\ \\| |    | | __/ _ \\/ __|")
print("    | |  | | | | (_| | | | | |/ ____ \\ |____| | ||  __/\\__ \\")
print("    |_|  |_| |_|\\__,_|_| |_|_/_/    \\_\\_____|_|\\__\\___||___/")
print("</pre>")

# Display inputs with colors
print("<p>")
print("Input 1:", input1)
print("Input 2:", input2)
print("</p>")

# HTML footer
print("</body>")
print("</html>")

sys.stdout.flush()
sys.stderr.flush()
sys.exit(0)
