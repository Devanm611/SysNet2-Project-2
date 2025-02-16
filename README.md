# Project 2 - HTTP Client
COP4635 - System & Networks 2
Team Members: Devan Rivera & Ashley Villegas

## Project Description
This project implements a multi-threaded HTTP server in C++ as well as a HTTP client to communicate with. 
The server dynamically assigns a port between 60001-60099, handles HTTP GET requests, and serves HTML and image files. 


It correctly returns:

The server runs indefinitely until manually stopped and supports multiple clients simultaneously using multithreading.
The client creates a socket, connects to the server through the socket, sends a GET request to the server, and shows the HTTP response.


**Required Files**
Make sure the following files are included in your project directory:

- index.html
- makefile
- img.jpg
- server.cpp
- client.cpp
- testPresence.html


## Running the Program (Using WSL: Ubuntu)

Step 1: Open 2 seperate WSL: Ubuntu terminals 

Step 2: In both terminals compile the makefile

        ~ Command: make

        ~ If there are any errors, 
          Run: make clean and rerun make

Step 3: Once compiled run the server executable in the first terminal
        ~ Executable: ./httpServer 

Step 4: In the second terminal run the client executable
        ~ Executable: ./httpClient

Step 5:

Step 6: Once your done, to stop the server and client press Ctrl + C 
        in the terminals where they are respectively running



**Alternative:(Using Remote Server (via SSH - Putty only))**

Step 1: Open 2 seperate SSH terminals

Step 2: compile the makefile in both terminals
        ~ Command: make

        ~ If there are any errors, 
          Run: make clean and rerun make

Step 3: Once compiled run the server executable in the first terminal
        ~ Executable: ./httpServer

Step 4: In the second terminal run the client executable
        ~ Executable: ./httpClient

Step 5: Once your done, to stop the server and client press Ctrl + C 
        in the terminals where they are respectively running


1. 

Expected Result (terminal):


2. 

Expected Result (terminal):



3. 

Expected Result (terminal):



Step 4: 
