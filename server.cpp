/*
 * Project 2 - HTTP Client
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
 #include <netdb.h>      // Hostname resolution functions
 
 using namespace std;
 
 #define BUFFER_SIZE 4096
 
 // Function to get the server's IP address
 string getServerIPAddress() {

    // Get the hostname
     char hostbuffer[256];
     struct hostent *host_entry;
 
     // Retrieve the hostname
     if (gethostname(hostbuffer, sizeof(hostbuffer)) == -1) {
         perror("gethostname");
         return "Unknown";
     }
 
     // Get the host information
     host_entry = gethostbyname(hostbuffer);
     if (!host_entry) {
         perror("gethostbyname");
         return "Unknown";
     }
 
     return inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
 }
 
 // Function to construct an HTTP response
 string getHTTPResponse(const string &status, const string &contentType, const string &body) {
     ostringstream HTTPResponse;
     HTTPResponse << "HTTP/1.1 " << status << "\r\n";
     HTTPResponse << "Content-Type: " << contentType << "\r\n";
     HTTPResponse << "Content-Length: " << body.size() << "\r\n";
     HTTPResponse << "Connection: close\r\n\r\n";
     HTTPResponse << body;
     return HTTPResponse.str();
 }
 
 // Function to determine Content-Type based on file extension
 string getContentHTTP(const string &filePath) {
     if (filePath.rfind(".html") != string::npos && filePath.rfind(".html") == filePath.size() - 5) return "text/html";
     
     if (filePath.rfind(".jpg") != string::npos && filePath.rfind(".jpg") == filePath.size() - 4) return "image/jpeg";
     
     if (filePath.rfind(".jpeg") != string::npos && filePath.rfind(".jpeg") == filePath.size() - 5) return "image/jpeg";
    
     if (filePath.rfind(".png") != string::npos && filePath.rfind(".png") == filePath.size() - 4) return "image/png";
     
     return "text/plain";
 }
 
 // Function to read file content
 string getFileContent(const string &filePath) {
     ifstream file(filePath, ios::in | ios::binary);
     if (file) {
         ostringstream content;
         content << file.rdbuf();
         file.close();
         return content.str();
     }
     return "";
 }
 
 // Function to handle client requests (Verbose)
 void handleClient(int clientSocket) {
     char buffer[BUFFER_SIZE];
     int bytesRead = read(clientSocket, buffer, BUFFER_SIZE - 1);
     
     // Check if the request is valid
     if (bytesRead <= 0) {
         cerr << "[ERROR] Failed to read request or client disconnected." << endl;
         close(clientSocket);
         return;
     }
 
     
     buffer[bytesRead] = '\0'; // Null-terminate the request data
     cout << "\n[INFO] Received Request:\n" << buffer << endl;
 
     istringstream request(buffer);
     string method, path, protocol;
     request >> method >> path >> protocol;
 
     // Check if the request is valid
     if (path == "/") path = "index.html";
     else path = path.substr(1);
 
     cout << "[INFO] Client requested: " << path << endl;
 
     string response;
     if (method == "GET") {
         string fileContent = getFileContent(path);
         if (!fileContent.empty()) { // File found

             response = getHTTPResponse("200 OK", getContentHTTP(path), fileContent);
             cout << "[INFO] Response: 200 OK (" << getContentHTTP(path) << ")" << endl;
         } else { // File not found

             response = getHTTPResponse("404 Not Found", "text/html", "<h1>404 Not Found</h1>");
             cout << "[INFO] Response: 404 Not Found" << endl;
         }
     } else { // Invalid request method

         response = getHTTPResponse("400 Bad Request", "text/html", "<h1>400 Bad Request</h1>");
         cout << "[INFO] Response: 400 Bad Request" << endl;
     }
 
     // Send the response back to the client
     write(clientSocket, response.c_str(), response.size());
     close(clientSocket);
     cout << "[INFO] Connection closed with client." << endl;
 }
 
 // Function to generate a random port
 int generatePort() {
     srand(time(NULL));
     return 60001 + (rand() % 99);
 }
 
 // Main function
 int main() {
    
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
     if (serverSocket < 0) {
         perror("Socket creation failed");
         exit(EXIT_FAILURE);
     }
 
     // Set server address
     struct sockaddr_in serverAddr;
     memset(&serverAddr, 0, sizeof(serverAddr));
     serverAddr.sin_family = AF_INET;
     serverAddr.sin_addr.s_addr = INADDR_ANY;
     int port = generatePort();
     serverAddr.sin_port = htons(port);
 
     // Bind the socket to the server address
     if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
         perror("Bind failed");
         close(serverSocket);
         exit(EXIT_FAILURE);
     }
 
     // Listen for incoming connections
     if (listen(serverSocket, 10) < 0) {
         perror("Listen failed");
         close(serverSocket);
         exit(EXIT_FAILURE);
     }
 
     // Print server information
     string ipAddress = getServerIPAddress();
     cout << "[INFO] Server running on IP: " << ipAddress << endl;
     cout << "[INFO] Server running on port: " << port << endl;
     cout << "[INFO] Access the server at: http://" << ipAddress << ":" << port << endl;
     cout << "[INFO] Waiting for client connections...\n" << endl;
 
     // Accept client connections
     while (true) {
         struct sockaddr_in clientAddr;
         socklen_t clientLen = sizeof(clientAddr);
         int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
         if (clientSocket < 0) {
             perror("[ERROR] Accept failed");
             continue;
         }
 
         // Handle client requests in a separate thread
         cout << "[INFO] Client connected from: " << inet_ntoa(clientAddr.sin_addr) << endl;
         thread t(handleClient, clientSocket);
         t.detach();
     }
 
     close(serverSocket);
     return 0;
 }
 