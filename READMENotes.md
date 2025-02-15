# Sysnet2-Project-2

**Client Notes** 

   *Socket Functions* 
1. socket() - Creates a new socket (endpoint for communication).
2. connect() - Connects a client socket to a server.
3. bind() - Binds a socket to an address and port (typically used by servers, not clients).
4. listen() - Marks a socket as passive, used for accepting connections (used in servers).
5. sendto() - Sends data to a specific address (used in UDP communication).
6. recvfrom() - Receives data from a specific address (used in UDP communication).
7. getaddrinfo() - Resolves a hostname into an address.
8. freeaddrinfo() - Frees the dynamically allocated memory used by getaddrinfo().
9. gethostname() - Retrieves the hostname of the local machine.
10. close() - Closes an open socket.

    *File I/O Functions (Data Handling)*
11. read() - Reads data from a file descriptor (e.g., socket).
12. write() - Writes data to a file descriptor (e.g., socket).
13. fflush() - Flushes buffered output data.
14. fscanf() - Reads formatted input from a stream (not commonly used in networking).
15. sprintf() - Formats a string and stores it in a buffer.

     *Signal Handling*
16. sigaction() - Configures signal handling, often used for handling interrupts or terminating connections gracefully.

     *Big Picture*
1. Create a Signal Handler (if needed)
2. Create the Client Socket and Connect to the Server
3. Send a HTTP GET Request
4. Recieve and Print the Response on the Client

     *Don't Forget*
1. Be Verbose with output
2. Create Test cases to check the program
3. Make a function to handle multiple requests (a loop that constantly sends requests)
4. Implement some Error Handling
5. If possible use HTTPS for encrypted communication
