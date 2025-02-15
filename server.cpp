/*
 * Project1 - HTTP Server
 * COP4635 System & Networks 2
 * Team Members: Devan Rivera, Ashley Villegas
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <thread> 
#include <netinet/in.h> //Socket address structures
#include <unistd.h> //System calls (read, write, close)
#include <sys/socket.h> //Socket functions
#include <arpa/inet.h> //IP address function

using namespace std;

#define BUFFER_SIZE 4096

//Function on HTTP response
string getHTTPResponse(const string &status, const string &contentType, const string &body){

    ostringstream HTTPResponse;
    HTTPResponse << "HTTP/1.1 " << status << "\r\n";
    HTTPResponse << "Content Type: " << contentType << "\r\n";
    HTTPResponse << "Content Length: " << body.size() << "\r\n";
    HTTPResponse << "Connection: close\r\n\r\n";
    HTTPResponse << body;
    return HTTPResponse.str();
    
}


//Content of the HTTP response
string getContentHTTP(const string &filePath){

    if(filePath.rfind(".html") != string::npos && filePath.rfind(".html") == filePath.size() - 5){
        return "text/html";
    }

     if(filePath.rfind(".jpg") != string::npos && filePath.rfind(".jpg") == filePath.size() - 4){
        return "image/jpeg";
    }

    if(filePath.rfind(".jpeg") != string::npos && filePath.rfind(".jpeg") == filePath.size() - 5){
        return "image/jpeg";
    }

    if(filePath.rfind(".png") != string::npos && filePath.rfind(".png") == filePath.size() - 4){
        return "image/png";
    }

    return "text/plain";

}

//Function to get the file content
string getFileContent(const string &filePath) {
    ifstream file(filePath, ios::in | ios::binary);
    if(file){
        ostringstream content;
        content << file.rdbuf();
        file.close();
        return content.str();
    } 
    else{
        return "";
    }
}

//Function to handle the client's connection
void handleClient(int clientSocket){
    
    char buffer[BUFFER_SIZE];
    read(clientSocket, buffer, BUFFER_SIZE); //Reading client request

    istringstream request(buffer);
    string method;
    string path;
    string protocol;

    request >> method >> path >> protocol;

    if(path == "/"){

        path = "index.html";

    }
    else{
        
        path = path.substr(1);
    }

    if(method == "GET"){

        ifstream file(path, ios::in | ios::binary);

        if(file){

            ostringstream body;
            body << file.rdbuf();
            file.close();

            string response = getHTTPResponse("200 OK", getContentHTTP(path), body.str());
            write(clientSocket, response.c_str(), response.size());

        }
        else{

            string response = getHTTPResponse("404 Not Found", "text/html", "<h1>404 Not Found</h1>");
            write(clientSocket, response.c_str(), response.size());

        }

    }
    else{

        string response = getHTTPResponse("400 Bad Request", "text/html", "<h1>400 Bad Request</h1>");
        write(clientSocket, response.c_str(), response.size());

    }

    close(clientSocket);

}

//Generate a random port number
int generateRandomPort(){

    srand(time(0)); //Seed the random number generator
    return  60001 + (rand() % 99) ; //Random port number between 6001 and 6099

}

int main(int argc, char *argv[]){

    int tcp_server_socket;
    int clientSocket;

    struct sockaddr_in tcp_server_address, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    cout << "Server starting..." << endl;

    //Create the Socket
    tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_server_socket < 0){

        perror("Socket creation failed");
        exit(EXIT_FAILURE);

    }
    cout << "Socket created successfully!" << endl;

    //Address server tructure and bind the socket
    memset(&tcp_server_address, 0, sizeof(tcp_server_address));
    tcp_server_address.sin_family = AF_INET; 
    tcp_server_address.sin_addr.s_addr = INADDR_ANY;   //Accept connections from any IP address

    int selectedPort = generateRandomPort(); //Generate a random port number
    tcp_server_address.sin_port = htons(selectedPort); //Convert the port number to network byte order
    cout << "Server assigned to port: " << selectedPort << endl;

    //Binding of server socket
    if(bind(tcp_server_socket, (struct sockaddr*)&tcp_server_address, sizeof(tcp_server_address)) < 0){

        perror("Socket bind() failed..."); //Print an error message if bind fails
        close(tcp_server_socket); 
        exit(EXIT_FAILURE);

    }
    cout << "Socket bind successful! Port: " << selectedPort << endl;

    //Listen for incoming connections
    if(listen(tcp_server_socket, 10) < 0){

        perror("Socket listen() failed..."); //Print an error message if listen fails
        close(tcp_server_socket); 
        exit(EXIT_FAILURE);

    }

    cout << "Server running on port: " << selectedPort << endl;
    cout << "Input in URL: http://localhost:" << selectedPort << endl;
    cout << "Note: press Ctrl + C to stop the server" << endl;
    cout << "Waiting for incoming connections..." << endl;

    //Accept incoming connections
    while(true){

        clientSocket = accept(tcp_server_socket, (struct sockaddr*)&clientAddr, &addrLen);
        
        if(clientSocket < 0){

            perror("Socket accept() failed..."); //Print an error message if accept fails
            close(tcp_server_socket);
            exit(EXIT_FAILURE);

        }
        else{

            cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << endl; 
            thread clientThread(handleClient, clientSocket);
            clientThread.detach(); // Detach the thread to handle the client independently
        
        }
    }

    close(tcp_server_socket);
    
    return 0;
}


 /*
        Devan Rivera 1/29/2025
        Worked on the Binding of the Socket
        Worked on the Listening of the Socket
        Worked on the Accepting of the incoming connection

        1/30/2025
        Fixed minor details
        Created a function to handle the client's connection
        Created a function to handle the HTTP response

        1/31/2025
        Worked on the creation of the HTTP response
        Started on the handleClient function
        Fixed minor details

    */


    /*
        Ashley Villegas:
        ~ 1/29/2025 ~
        - Worked on the creation of the socket
        - working on adjusting bind and server structure

        ~ 1/31/2025 ~
        - Worked on the adjusting listen and accept
        - Worked on the generation of the random port number (dynamic)

        ~ 2/1/2025 ~
        - Worked on client connection
        - Worked on the display of the server's port number
        - Worked on file content and HTTP response
        - fixed minor details
    
    */