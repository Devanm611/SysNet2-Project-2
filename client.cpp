/*
 * Project 2 - HTTP Client
 * COP4635 System & Networks 2
 * Team Members: Devan Rivera, Ashley Villegas
 */

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h> // System calls: close
#include <arpa/inet.h> // IP address functions
#include <sys/socket.h> // Socket functions

using namespace std;

#define BUFFER_SIZE 4096

int main(int argc, char* argv[]) { 

    if (argc < 3) {// Check if the user provided the server IP and port
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

        if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {// Convert IP address to binary form
            
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

        // Receive the response from the server
        char buffer[BUFFER_SIZE];
        int bytesReceived;
        cout << "\n--- Response from Server ---\n";

        while ((bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) { // Receive data until there is no more data to read
            buffer[bytesReceived] = '\0';
            cout << buffer;
        }
        cout << "\n--- End of Response ---\n";

        // Close the connection before making another request
        close(clientSocket);
    }

    return 0;
}
