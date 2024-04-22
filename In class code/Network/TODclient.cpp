#include <cstdlib>
#include <iostream>

#include <netdb.h>
#include <unistd.h>

using namespace std;

// prototype
int do_client (char *server);


int main (int argc, char*argv[])
{
    // command line error handling
    if (argc!=2)
    {
        cerr << "USAGE: " << argv[0] << "<tod-server>\n";
        return 1;
    }

    // return the server name when invoked
    return do_client(argv[1]);
}


int do_client (char *server)
{
    //cout << "Connecting to " << server << endl;

    // variable to hold return values from various network calls 
    int rval = 0;

    // holds IP information about our server
    struct hostent *serverEntry;
    
    // holds socket information for our connection with server
    // technicaly a "socket," which holds IP (IP layer) and port 
    // (TCP info) for both server (foreign end) and client (local end)
    struct sockaddr_in serverInfo;

    // convert server "name" to "numeric" IP address
    serverEntry = gethostbyname(server);

    // cannot connect to the server
    if (!serverEntry)
    {
        cerr << "gethostbyname failed for: " << server << endl;
        return 1;
    }


    // create an actual network socket
    //  AF_INET --> IPv4
    //  SOCK_STREAM --> TCP
    //  0 --> use default "flag"
    int mySock = socket(AF_INET, SOCK_STREAM, 0);

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr = * ( (struct in_addr *)serverEntry -> h_addr_list[0] );
    serverInfo.sin_port = htons(13 /**port number*/); 
    

    // attempt to connect to the server
    rval = connect(mySock, (struct sockaddr *) &serverInfo, sizeof(serverInfo) );

    if (rval<0)
    {
        cerr << "connect() failed. \n";
        return 1;
    }

    //if we get here we are connected to the server !!!!!

    int n=0; // how many bytes did we just read? 

    char recvln[81]; // actual bytes (character) read (BUFFER)

    // while there are stuff read from the connection
    while (  ( n=read(mySock, recvln, 80 /* one less to store end line terminator */)  ) > 0 ) 
    {
        recvln[n] = '\0'; // tack on null terminate returned "string" 
        cout << recvln; // print what we read out
    }

    cout << endl; 

    //end network connection
    close (mySock);

    return 0;
}
