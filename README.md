# Project 2 - HTTP Client
COP4635 - System & Networks 2
Team Members: Devan Rivera & Ashley Villegas

## Project Description
This project implements a multi-threaded HTTP server in C++ and an HTTP client that interacts with the server using HTTP GET requests. The server dynamically assigns a random port within the range 60001-60099 and serves multiple client connections simultaneously using multi-threading. It is capable of handling HTML, JPEG, and PNG file requests, returning appropriate HTTP responses. As well the client allows users to specify a server IP and port, send HTTP GET requests for specific files, and display the server's responses within the terminal. Users can manually terminate the client when finished.


## Features
- The server supports concurrent clients using multi-threading.

- The server handles HTTP GET requests for HTML and image files (JPG, PNG).

- The client allows users to browse files on the server and request them dynamically.

- The server runs indefinitely until manually stopped.


**Required Files**
Make sure the following files are included in your project directory:

- index.html
- makefile
- img.jpg
- server.cpp
- client.cpp
- testPresence.html


## Running the Program (Using WSL: Ubuntu or via SSH - Putty)

Step 1: Open 2 seperate WSL: Ubuntu terminals 

Step 2: In one terminal, compile the makefile

        ~ Command: make

        ~ If there are any errors, run:
          Run: make clean and rerun make

Step 3: Once compiled, run the server executable in the terminal
        ~ Executable: ./httpServer 

        Note: It will provide you a Server IP and port number

Step 4: In the second terminal, go to your directory where all your files and executables are   
        located and run the client executable includinfg the IP and port number of the server.
        ~ Executable: ./httpClient <Server_IP> <Server_PORT #>

        ~ Example: ./httpClient 127.0.1.1 60016

Step 5: Where the client is beind executed it will ask the user to "Enter the filename to request
        (or type 'exit' to quit)" which from there the user can enter the provided files to test
        the program.

        Example: Enter the filename to request (or type 'exit' to quit): index.html

Step 6: Once the user is done, in the client terminal type "exit" to disconnect the server 
        connection and stop the program. As for the server terimnal side press Ctrl + C to 
        stop the server from running.

