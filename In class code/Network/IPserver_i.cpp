#include <cstdlib>
#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <unistd.h>

//using namespace std; #comment out if bind does not work

#define MAX_WAITING 25 

// prototypes
int do_server(unsigned int port);
void do_work(unsigned with_sock, struct sockaddr_in *fromClient);

//returns the port number
int main (int argc, char*argv[])
{
    if (argc!=2)
    {
        std:: cerr << "USAGE: " << argv[0] << "<port number>" << std:: endl;
        return 1;
    }

    //cout << "Attempting to run server on port : " << stoi(argv[1]) << endl;
    return do_server (std::stoi(argv[1]));
}

//run server on port number
int do_server(unsigned int port)
{
    int listen_sock; 

    // local address in the socket
    struct sockaddr_in local_addr;

    //create socket
    listen_sock = socket(AF_INET /* IPv4*/, SOCK_STREAM /*TCP*/, 0);
    if (listen_sock<0)
    {
        std::cerr << "could not create socket\n";
        return 1;
    }

    local_addr.sin_family = AF_INET; // use IPv4
    local_addr.sin_addr.s_addr = INADDR_ANY; // use "wildcard" IP address
    local_addr.sin_port = htons(port); 

    //hold value from connection
    //take address and bind it with the listening socket
    int rval = bind(listen_sock, (struct sockaddr *) &local_addr, sizeof(local_addr));

    if (rval != 0)
    {
        std::cerr << "Binding failed - possible causes include:\n"
             << "   * invalid port number (access denied or already in use?)\n"
             << "   * invalid local address (did you use the wildcard?)\n";

        return 1;
    }

    // sitting here waiting for client to connect... "listening"
    rval = listen(listen_sock, MAX_WAITING);
    if (rval!=0)
    {
        std::cerr << "listen() failed\n";
        return 1;
    }

    while(true)
    {
        // accept incoming client
        int accepted_socket;        // socket for actual connected client
        struct sockaddr_in from;    // holds client address data
        unsigned int from_len;      // bytes in from

        from_len = sizeof(from);
        
        //flags when it gets a valid socket
        accepted_socket = accept(listen_sock,
                                (struct sockaddr*) &from,                              
                                &from_len);

        // process imcoming client
        do_work(accepted_socket, &from);

    }

    return 0;
}

void do_work(unsigned with_sock, struct sockaddr_in *fromClient)
{
    std::cout << "Got a connecction!\n";

    std::string buffer; // the result we are trying to send back to client
    buffer = "Your IP is ";
    buffer += inet_ntoa(fromClient->sin_addr); // turn ip to a string
    buffer += "\n";

    char *cbuff = (char *) buffer.c_str(); // network code needs array of bytes (chars)

    int needed = buffer.length();

    while (needed > 0)
    {
        int n = write (with_sock, cbuff, needed); // number sent
        needed -= n; // update amount still need to send out
        cbuff += n; // save pointer location
    }

    std::cout << "Processed a connection from " << inet_ntoa(fromClient->sin_addr) << std::endl;

    // close the connected (accepted) socket from sever side
    close(with_sock);
}
