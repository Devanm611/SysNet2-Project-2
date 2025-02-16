/*
 * Project2 - HTTP Client
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
 #include <sys/socket.h> //API and definitions for the sockets
 #include <sys/types.h>  //more definitions
 #include <netinet/in.h> //Structures to store address information
 #include <arpa/inet.h> //IP address function

 #define BUFFER_SIZE 4096 

 using namespace std;

//Basic TCP Client: socket() creation > connect() > receive() > display > close

int main(int argc, char* argv[]){

    if(argc < 3){   //The client expects two arguments (IP and Port #)

        cerr << "Usage: " << argv[0] << " <IP> <Port #>" << endl;   

        return 1;
    }

    string serverIP = argv[1];
    int serverPort = atoi(argv[2]);

    //creating the TCP socket
    int tcp_client_socket;                                    //Socket descriptor
    tcp_client_socket = socket(AF_INET, SOCK_STREAM, 0);      //Calling the socket function - args: socket domain, socket stream type, TCP protocol (default)

    if(tcp_client_socket < 0){
     
        perror("Socket creation failed");   
        exit(EXIT_FAILURE);

    }    

    //specify address and port of the remote socket
    struct sockaddr_in tcp_server_address;             //declaring a structure for the address
    tcp_server_address.sin_family = AF_INET;           //Structure Fields' definition: Sets the address family of the address the client would connect to
    tcp_server_address.sin_port = htons(serverPort);  //Specify and pass the port number to connect - converting in right network byte order
    tcp_server_address.sin_addr.s_addr = INADDR_ANY;   //Connecting to 0.0.0.0

    if(inet_pton(AF_INET, serverIP.c_str(), &tcp_server_address.sin_addr) <= 0){   //Converts the IP address from text to binary form
        
        cerr << "Invalid IP Address" << endl;
        close(tcp_client_socket);

        return 1;
    }

    //connecting to the remote socket using information provided from server
    int connection_status = connect(tcp_client_socket, (struct sockaddr *) &tcp_server_address, sizeof(tcp_server_address));     //params: which socket, cast for address to the specific structure type, size of address
    if (connection_status == -1){ 
                                                                                                //return value of 0 means all okay, -1 means a problem
        printf(" Problem connecting to the socket! Sorry!! \n");
        close(tcp_client_socket);

    }
    else{

        printf("Connected to the server! \n");

    }

    //Sends a complete HTTP request
    ostringstream http_request;   //Creating a string stream to build the HTTP GET request
    http_request << "GET / HTTP/1.1" << endl;   //HTTP request line
    http_request << "Host: " << serverIP << ":" << serverPort << endl;   //Host header
    http_request << "Connection: close" << endl;   //Closes the connection
    string request = http_request.str();   //Convert the string stream to a string



    char tcp_server_response[256];
    recv(tcp_client_socket, &tcp_server_response, sizeof(tcp_server_response), 0);   // params: where (socket), what (string), how much - size of the server response, flags (0)

    //Output, as received from Server
    printf("\n\n Server says: %s \n", tcp_server_response);

    //closing the socket
    close(tcp_client_socket);

    return 0;
}

/*
        Devan Rivera 2/14/2025
        Cloned the previous Repository to utilize the server.cpp file
        Utilized the code from the TCP Client example and built 
        upon the princple of creating a client.
        Implemented the socket creation, connection, and request 
        of Http response.

        2/15/2025
        
        2/16/2025

*/