# Sysnet2-Project-1

**Server Notes** 

ls = socket(port#, IP addr)

bind(ls,...) //helps tie application layer to transport layer

listen (ls,...) //check constantly for a connection (creation of socket must be made on both sides)

some infinite loop

s = accept(ls,...)

block

unblock

Server Data - Transfer

Handle Request and Create Response

closes and returns to loop

**For Socket creation**

struct sockaddr_in{

    u_char          sin_len;

    //normally not used
    u_short         sin_family;
    u_short         sin_port;
    struct in_addr  sin_addr;
    char            sin_zero[8]
    //normally not used

};

**Creating a Socket**
int socket(

    int domain, /*AF_INET*/
    int type, /*SOCK_DGRAM, SOCK_STREAM*/
    int protocol /*0*/

);

(Return value of -1 indicates an error)

socketFD = socket (AF_INET, SOCK_DGRAM, 0);
if(socketFD < 0){

    perror("sendUDP:socket");   //error handling for socket
    return -1;

}

(Obtaining Host Information)

hostptr = gethostbyname(argv[1]);
if(hostptr == NULL){

    perror("sendUDP:gethostbyname");
    return -1;

}

(Function makes a call to DNS and returns a struct called hostent)

memset(&destaddr, 0, sizeof(struct sockaddr_in));
destaddr.sin_family = AF_INET;
memcpy(&destaddr.sin_addr, hostptr->h_addr, hostptr->h_length);
destaddr.sin_port = htons((u_short)atoi(argv[2]));

(In Order to Send information)

fgets(buffer, 256, stdin);
sendlen = strlen(buffer) - 1;
bytes = sendto(

    socketFD,
    buffer,
    (size_t)sendlen,
    0,
    (struct sockkaddr *)&destaddr,
    (socklen_t)sizeof(destaddr)

);

(Receiver Extra Step)

- Receiver must have a port number
- Delivery:
    - Packet->Machine(host)->OS->Process
- The bind() system call provides "name" for socket.
- Can send without a "named socket", but must have "named socket" to receive.

int bind(

    int sockFD,
    struct sockaddr *my_addr,
    socklen_t addrlen

);

struct sockaddr_in{

    u_char sin_len;

    u_short sin_family;
    u_short sin_port;
    struct in_addr sin_addr;

    char   sin_zero[8];

};

**Static Binding**

- User provides port number; some options
- Hardcoded in program
DA.sin_port = htons(51664);
    
- Provided on command-line
DA.sin_port = htons((u_shrot)atoi(argv[1]));
    
- Provided user input
DA.sin_port = htons((u_short)atoi(inString));

**Announcing Socket Name**

printf("name: %s\n", hostptr->h_name);

printf("addr: %s\n", inet_ntoa*(srcAddr.sin_addr));

printf("port: %d\n", ntohs(srcAddr.sin_port));

**Dyanamic Binding**

- Kernel provides port number
- Send special "you pick" to kernel
- Value of 0(zero) is indicator

DA.sin_port = 0;

- Kernel picks next available port number 
- Stores port number internally 

int bind(

    int sockFD,
    struct sockaddr *my_addr,   //The bind() selects next available port 
    socklen_t addrlen

);

int getsockname(

    int sockFD,
    struct sockaddr *my_addr,
    socklen_t *addrlen

);

**Given Hostname**

struc hostent * gethostbyname(char* name)

struct hostent{

    char *h_name; /* official name */
    char **h_aliases; /* list of alises */
    int  h_addrtype;  /* PF_INET */
    int h_length; /* lentgh. e.g. 4 */
    char **h_addr_list; /* list of addr. */

};

**Big Picture**

1. Create a socket.
2. Get informatioon about destination host.
3. Fill in destation address structure.
4. Connect to remote host.
5. Send/Receive data to dest.
6. Close connection.