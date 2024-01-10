#!/usr/bin/env python3
import cgi
import cgitb
import os
import json

# Define your database file
DATABASE_FILE = "/Users/abdeel-o/Desktop/Webserv/database/database.json"

def load_users():
    # Load user data from the database file
    if os.path.exists(DATABASE_FILE):
        with open(DATABASE_FILE, 'r') as file:
            return json.load(file)
    else:
        return {}

def print_users(users):
    # Print the list of users
    print("<ul>")
    for email, password in users.items():
        print("<li>{}: {}</li>".format(email, password))
    print("</ul>")

def save_users(users):
    # Save user data to the database file
    with open(DATABASE_FILE, 'w') as file:
        json.dump(users, file)

def authenticate_user(email, password, users):
    # Check if the user exists in the database
    if email in users and users[email] == password:
        return True
    else:
        return False

def register_user(email, password, users):
    # Register a new user in the database
    users[email] = password
    save_users(users)

def serve_html_file(file_path):
    # Read the content of the HTML file and print it
    with open(file_path, 'r') as file:
        print("Content-type: text/html\n")
        print(file.read())

def main():
    # Load existing users from the database
    users = load_users()

    # Get form data
    form = cgi.FieldStorage()
    email = form.getvalue('email')
    password = form.getvalue('pass')

    # Check if the form is for registration or login
    if 'register' in form: # 'register' is the name of the submit button
        # Register a new user
        register_user(email, password, users)
        serve_html_file("/Users/abdeel-o/Desktop/Webserv/www/auth_pages/register.html")
    else:
        # Check if the user is authenticated
        if authenticate_user(email, password, users):
            # Set a cookie to indicate the user is logged in
            print("Set-Cookie: user_email={}; Path=/".format(email))
            # Print success message
            serve_html_file("/Users/abdeel-o/Desktop/Webserv/www/auth_pages/succ_login.html")
        else:
            serve_html_file("/Users/abdeel-o/Desktop/Webserv/www/auth_pages/fail_login.html")
             

if __name__ == "__main__":
    main()
