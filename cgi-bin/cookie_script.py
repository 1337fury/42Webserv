#!/usr/bin/env python

import cgi
import cgitb
import os

cgitb.enable()


# Function to set a cookie
def set_cookie(cookie_name, cookie_value):
    print("Set-Cookie: {}={}; Path=/".format(cookie_name, cookie_value)) # Set the cookie name and value and specify the path to the root directory of the website (i.e. /) so that the cookie is sent with every request to the website (i.e. every page)

# Retrieve form data
form = cgi.FieldStorage()
cookie_name = form.getvalue('cookie_name')
cookie_value = form.getvalue('cookie_value')

# HTML header
# print("Content-type: text/html \r\n\r\n")

# Check if both cookie name and value are provided
if cookie_name and cookie_value:
    # Set the cookie
    set_cookie(cookie_name, cookie_value)

# HTML response
print("""
<html>
<head>
    <title>Cookie Script</title>
</head>
<body>
    <h2>Cookie Set Successfully</h2>
    <p>Cookie Name: {}</p>
    <p>Cookie Value: {}</p>
</body>
</html>
""".format(cookie_name, cookie_value))
