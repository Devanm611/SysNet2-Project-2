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
 #include <unistd.h>       // System calls: close()
 #include <arpa/inet.h>    // IP address functions
 #include <sys/socket.h>   // Socket functions
 
 using namespace std;
 
 #define BUFFER_SIZE 4096  // Buffer size for reading data from the server
 
 // Function to save binary file (for images)
 void saveBinaryFile(const string &filename, const char *data, size_t size) {
     ofstream file(filename, ios::binary);
     if (!file) {
         cerr << "[ERROR] Could not save file: " << filename << endl;
         return;
     }
     file.write(data, size);
     file.close();
     cout << "[INFO] Image saved as: " << filename << endl;
 }
 
 // Function to handle and process HTTP responses
 void processHttpResponse(int clientSocket) {
     char buffer[BUFFER_SIZE];
     string response;
     int bytesReceived;
     bool isImage = false;
     string contentType;
     size_t contentLength = 0;
     ofstream imageFile;
 
     // Read response from the server
     while ((bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
         buffer[bytesReceived] = '\0';
         response += buffer;
 
         // Look for end of HTTP headers
         size_t headerEnd = response.find("\r\n\r\n");
         if (headerEnd != string::npos) {
             string headers = response.substr(0, headerEnd);
             cout << "\n--- HTTP Headers ---\n" << headers << "\n";
 
             // Detect if response is an image
             if (headers.find("Content-Type: image") != string::npos) {
                 isImage = true;
 
                 // Extract Content-Length (if present)
                 size_t lengthPos = headers.find("Content-Length: ");
                 if (lengthPos != string::npos) {
                     stringstream ss(headers.substr(lengthPos + 16));
                     ss >> contentLength;
                 }
 
                 // Extract content type
                 size_t typePos = headers.find("Content-Type: ");
                 if (typePos != string::npos) {
                     contentType = headers.substr(typePos + 14);
                     contentType = contentType.substr(0, contentType.find("\r"));
                 }
 
                 // Save image data
                 string filename = "downloaded_image";
                 if (contentType.find("jpeg") != string::npos) filename += ".jpg";
                 else if (contentType.find("png") != string::npos) filename += ".png";
 
                 imageFile.open(filename, ios::binary);
                 if (!imageFile) {
                     cerr << "Could not create file: " << filename << endl;
                     return;
                 }
 
                 // Write remaining data after headers
                 imageFile.write(buffer + headerEnd + 4, bytesReceived - headerEnd - 4);
             } else {
                 // If not an image, print the response body
                 cout << "\n--- HTTP Body ---\n" << response.substr(headerEnd + 4) << "\n";
                 return;
             }
             break;
         }
     }
 
     // If it's an image, continue receiving the rest of the binary data
     if (isImage) {
         while ((bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
             imageFile.write(buffer, bytesReceived);
         }
         imageFile.close();
         cout << "Image file saved successfully." << endl;
     }
 }
 
 int main(int argc, char* argv[]) { 
     if (argc < 3) {
         cerr << "Usage: " << argv[0] << " <Server IP> <Server Port>" << endl;
         return 1;
     }
 
     string serverIP = argv[1];
     int serverPort = atoi(argv[2]);
 
     cout << "Connecting to server at IP: " << serverIP << " on port: " << serverPort << endl;
 
     while (true) {
         // Create socket
         int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
         if (clientSocket < 0) {
             perror("Socket creation failed");
             exit(EXIT_FAILURE);
         }
 
         // Set server address
         struct sockaddr_in serverAddr;
         serverAddr.sin_family = AF_INET;
         serverAddr.sin_port = htons(serverPort);
 
         if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
             cerr << "Invalid IP Address" << endl;
             close(clientSocket);
             return 1;
         }
 
         // Connect to the server
         if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
             perror("Connection failed");
             close(clientSocket);
             return 1;
         }
 
         cout << "Connected to the server!" << endl;
 
         // Prompt user for file request
         string fileName;
         cout << "Enter the filename to request (or type 'exit' to quit): ";
         getline(cin, fileName);
 
         if (fileName == "exit") { 
             cout << "Exiting client..." << endl;
             close(clientSocket);
             break;  // Exit the loop
         }
 
         // Construct the GET request
         string request = "GET /" + fileName + " HTTP/1.1\r\nHost: " + serverIP + ":" + to_string(serverPort) + "\r\nConnection: close\r\n\r\n";
         send(clientSocket, request.c_str(), request.size(), 0);
 
         // Process server response
         processHttpResponse(clientSocket);
 
         // Close the connection before making another request
         close(clientSocket);
     }
 
     return 0;
 }