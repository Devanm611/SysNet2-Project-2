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
 #include <cstdlib>
 #include <unistd.h>      // System calls: close
 #include <arpa/inet.h>   // IP address functions
 #include <sys/socket.h>  // Socket functions
 
 using namespace std;
 
 #define BUFFER_SIZE 4096
 
 // Function to save an image file received from the server
 void saveBinaryFile(const string &filename, const char *data, int size) {
     ofstream file(filename, ios::binary);
     if (!file) {
         cerr << "[ERROR] Could not create file " << filename << endl;
         return;
     }
     file.write(data, size);
     file.close();
     cout << "[INFO] Image saved as " << filename << endl;
 }
 
 // Function to process server responses
 void processHttpResponse(int sockfd) {
     char buffer[BUFFER_SIZE];
     string response;
     int bytes_received;
     string contentType;
     bool isImage = false;
     bool isText = false;
     ofstream imageFile;
 
     // Read response from the server
     while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
         response.append(buffer, bytes_received);
 
         // Look for end of HTTP headers
         size_t headerEnd = response.find("\r\n\r\n");
         if (headerEnd != string::npos) {
             string headers = response.substr(0, headerEnd);
             cout << "\n--- HTTP Headers ---\n" << headers << "\n";
 
             // Detect Content-Type
             size_t typePos = headers.find("Content-Type: ");
             if (typePos != string::npos) {
                 contentType = headers.substr(typePos + 14);
                 contentType = contentType.substr(0, contentType.find("\r"));
             }
 
             // Check if it's an image or text
             if (contentType.find("image") != string::npos) {
                 isImage = true;
                 string filename = "downloaded_image";
 
                 // Determine the file extension
                 if (contentType.find("jpeg") != string::npos) filename += ".jpg";
                 else if (contentType.find("png") != string::npos) filename += ".png";
                 else if (contentType.find("gif") != string::npos) filename += ".gif";
 
                 // Open file for writing
                 imageFile.open(filename, ios::binary);
                 if (!imageFile) {
                     cerr << "[ERROR] Could not create file: " << filename << endl;
                     return;
                 }
                 // Write remaining data after headers
                 imageFile.write(buffer + headerEnd + 4, bytes_received - headerEnd - 4);
                 cout << "[INFO] Image saved as " << filename << endl;
             } 
             else if (contentType.find("text") != string::npos) { 
                 isText = true;
                 cout << "\n--- HTML Response Body ---\n" << response.substr(headerEnd + 4) << endl;
             }
             break;
         }
     }
 
     // If it's an image, continue receiving the rest of the binary data
     if (isImage) {
         while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
             imageFile.write(buffer, bytes_received);
         }
         imageFile.close();
         cout << "[INFO] Image file saved successfully." << endl;
     }
 
     // Print additional text if needed
     if (isText) {
         while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
             buffer[bytes_received] = '\0';
             cout << buffer;
         }
         cout << "\n--- End of HTML Response ---\n";
     }
 }
 
 int main(int argc, char* argv[]) { 
     if (argc < 3) { // Check if the user provided the server IP and port
         cerr << "Usage: " << argv[0] << " <Server IP> <Server Port>" << endl;
         return 1;
     }
 
     string serverIP = argv[1];
     int serverPort = atoi(argv[2]);
 
     cout << "Connecting to server at IP: " << serverIP << " on port: " << serverPort << endl;
 
     while (true) {
         // Create a new socket for each request
         int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
         if (clientSocket < 0) {
             perror("Socket creation failed");
             exit(EXIT_FAILURE);
         }
 
         // Set server address
         struct sockaddr_in serverAddr;
         serverAddr.sin_family = AF_INET;
         serverAddr.sin_port = htons(serverPort);
 
         if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) { // Convert IP address to binary form
             cerr << "Invalid IP Address" << endl;
             close(clientSocket);
             return 1;
         }
 
         if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) { // Connect to the server
             perror("Connection failed");
             close(clientSocket);
             return 1;
         }
 
         cout << "Connected to the server!" << endl;
 
         string fileName;
         cout << "Enter the filename to request (or type 'exit' to quit): ";
         getline(cin, fileName);
 
         if (fileName == "exit") { 
             cout << "Exiting client..." << endl;
             close(clientSocket);
             break; // Exit the loop when user types 'exit'
         }
 
         // Send the request to the server
         string request = "GET /" + fileName + " HTTP/1.1\r\nHost: " + serverIP + ":" + to_string(serverPort) + "\r\nConnection: close\r\n\r\n";
         send(clientSocket, request.c_str(), request.size(), 0);
 
         // Process the server response
         processHttpResponse(clientSocket);
 
         // Close the connection before making another request
         close(clientSocket);
     }
 
     return 0;
 }
 