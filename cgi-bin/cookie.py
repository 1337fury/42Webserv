#!/usr/bin/env python3
import cgi
import os


# Function to get cookie value
def get_cookie_value(cookie_name):
    if "HTTP_COOKIE" in os.environ:
        cookies = os.environ["HTTP_COOKIE"]
        cookie_list = cookies.split("; ")
        for cookie in cookie_list:
            cookie_parts = cookie.split("=")
            if len(cookie_parts) == 2:
                name, value = cookie_parts
                if name == cookie_name:
                    return value
    return None

# Get name and session from query parameters or cookies
name = cgi.FieldStorage().getvalue('name') or get_cookie_value('name')
session = cgi.FieldStorage().getvalue('session') or get_cookie_value('session')

# Set cookies
if name:
    print(f"Set-Cookie: name={name}")
if session:
    print(f"Set-Cookie: session={session}")

# Print content type and start HTML
print("Content-Type: text/html\n\n")
print("<html><head><title>Greetings</title></head><body>")

# Display appropriate message
if name or session:
    if name:
        print(f"<h1>Name, {name}!</h1>")
    if session:
        print(f"<h1>Session, {session}!</h1>")
    print("</body></html>")
else:
    print("<h1>Please enter your name.</h1>")
    print('<form method="GET" action="cookie.py">')
    print('Name: <input type="text" name="name">')
    print('Session: <input type="text" name="session">')
    print('<input type="submit" value="Submit">')
    print('</form>')
    print("</body></html>")
