/*
 * Project 2 - HTTP Server
 * COP4635 System & Networks 2
 * Team Members: Devan Rivera, Ashley Villegas
 */

 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include <string>
 #include <cstring>
 #include <thread>
 #include <netinet/in.h> // Socket structures
 #include <unistd.h>     // System calls: read, write, close
 #include <sys/socket.h> // Socket functions
 #include <arpa/inet.h>  // IP address functions
 #include <netdb.h>  // Required for `gethostbyname()`
 
 using namespace std;
 
 #define BUFFER_SIZE 4096 // Buffer size for reading files and data
 
 // Function to retrieve the server's IP address
 string getServerIPAddress() {
     char hostbuffer[256];
     struct hostent *host_entry;
 
     // Get the hostname of the system
     if (gethostname(hostbuffer, sizeof(hostbuffer)) == -1) {
         perror("gethostname");
         return "Unknown";
     }
 
     // Get host information
     host_entry = gethostbyname(hostbuffer);
     if (!host_entry) {
         perror("gethostbyname");
         return "Unknown";
     }
 
     return inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
 }
 
 // Function to determine Content-Type based on file extension
 string getContentHTTP(const string &filePath) {
    if (filePath.rfind(".html") == filePath.size() - 5) return "text/html";
    if (filePath.rfind(".jpg") == filePath.size() - 4 || filePath.rfind(".jpeg") == filePath.size() - 5) return "image/jpeg";
    if (filePath.rfind(".png") == filePath.size() - 4) return "image/png";
    return "text/plain"; // Default type for unknown files
}
 
 // Function to send HTTP headers
 void sendHTTPHeader(int clientSocket, const string &status, const string &contentType, size_t contentLength) {
     ostringstream response;
     response << "HTTP/1.1 " << status << "\r\n";
     response << "Content-Type: " << contentType << "\r\n";
     response << "Content-Length: " << contentLength << "\r\n";
     response << "Connection: close\r\n\r\n";
 
     string headerStr = response.str();
     send(clientSocket, headerStr.c_str(), headerStr.size(), 0);
 }
 
 // Function to serve a file to the client (handles both text and binary data)
 void serveFile(int clientSocket, const string &filePath) {
     ifstream file(filePath, ios::in | ios::binary | ios::ate);
 
     // Check if file exists
     if (!file) {
         string errorResponse = "<h1>404 Not Found</h1>";
         sendHTTPHeader(clientSocket, "404 Not Found", "text/html", errorResponse.size());
         send(clientSocket, errorResponse.c_str(), errorResponse.size(), 0);
         cout << "[INFO] Response: 404 Not Found" << endl;
         return;
     }
 
     // Get file size and content type
     size_t fileSize = file.tellg();
     file.seekg(0, ios::beg);
     string contentType = getContentHTTP(filePath);
 
     // Send HTTP headers
     sendHTTPHeader(clientSocket, "200 OK", contentType, fileSize);
     cout << "[INFO] Response: 200 OK (" << contentType << ")" << endl;
 
     // Send file content in chunks
     char buffer[BUFFER_SIZE];
     while (file.read(buffer, sizeof(buffer))) {
         send(clientSocket, buffer, file.gcount(), 0);
     }
     send(clientSocket, buffer, file.gcount(), 0); // Send remaining bytes
 
     file.close();
 }
 
 // Function to handle incoming client requests
 void handleClient(int clientSocket) {
     char buffer[BUFFER_SIZE];
     int bytesRead = read(clientSocket, buffer, BUFFER_SIZE - 1);
 
     // If reading fails, close the connection
     if (bytesRead <= 0) {
         cerr << "[ERROR] Failed to read request or client disconnected." << endl;
         close(clientSocket);
         return;
     }
 
     // Null-terminate the request buffer
     buffer[bytesRead] = '\0';
     cout << "\n[INFO] Received Request:\n" << buffer << endl;
 
     istringstream request(buffer);
     string method, path, protocol;
     request >> method >> path >> protocol;
 
     // Default to "index.html" if root path is requested
     if (path == "/") path = "index.html";
     else path = path.substr(1); // Remove leading "/"
 
     cout << "[INFO] Client requested: " << path << endl;
 
     // Only handle GET requests
     if (method == "GET") {
         serveFile(clientSocket, path);
     } else {
         string errorResponse = "<h1>400 Bad Request</h1>";
         sendHTTPHeader(clientSocket, "400 Bad Request", "text/html", errorResponse.size());
         send(clientSocket, errorResponse.c_str(), errorResponse.size(), 0);
         cout << "[INFO] Response: 400 Bad Request" << endl;
     }
 
     // Close client connection
     close(clientSocket);
     cout << "[INFO] Connection closed with client." << endl;
 }
 
 // Function to generate a random port number
 int generatePort() {
     srand(time(NULL));
     return 60001 + (rand() % 99); // Generates a random port between 60001 - 60099
 }
 
 // Main function to start the server
 int main() {
     // Create the server socket
     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
     if (serverSocket < 0) {
         perror("Socket creation failed");
         exit(EXIT_FAILURE);
     }
 
     // Set up the server address structure
     struct sockaddr_in serverAddr;
     memset(&serverAddr, 0, sizeof(serverAddr));
     serverAddr.sin_family = AF_INET;
     serverAddr.sin_addr.s_addr = INADDR_ANY;
     int port = generatePort();
     serverAddr.sin_port = htons(port);
 
     // Bind the socket to the address and port
     if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
         perror("Bind failed");
         close(serverSocket);
         exit(EXIT_FAILURE);
     }
 
     // Listen for incoming client connections
     if (listen(serverSocket, 10) < 0) {
         perror("Listen failed");
         close(serverSocket);
         exit(EXIT_FAILURE);
     }
 
     // Display server startup information
     string ipAddress = getServerIPAddress();
     cout << "[INFO] Server running on IP: " << ipAddress << endl;
     cout << "[INFO] Server running on port: " << port << endl;
     cout << "[INFO] Access the server at: http://" << ipAddress << ":" << port << endl;
     cout << "[INFO] Waiting for client connections...\n" << endl;
 
     // Infinite loop to accept client connections
     while (true) {
         struct sockaddr_in clientAddr;
         socklen_t clientLen = sizeof(clientAddr);
         int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
         if (clientSocket < 0) {
             perror("[ERROR] Accept failed");
             continue;
         }
 
         // Log client connection and handle in a new thread
         cout << "[INFO] Client connected from: " << inet_ntoa(clientAddr.sin_addr) << endl;
         thread t(handleClient, clientSocket);
         t.detach();
     }
 
     // Close the server socket (never actually reached in infinite loop)
     close(serverSocket);
     return 0;
 }
 