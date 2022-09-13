#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

using namespace std;

class Server{

private:
    // connection --> boolean that indicates if the client and server are actually texting
    // serverSocket --> save the configuration of the server socket
    // newSocket --> communication channel with a client
    int connection, serverSocket, newSocket;

    // address --> struct with int attributes for configure later the parameters of the server socket
    struct sockaddr_in address;

    // opt --> this value is necessary to proof if the port is already use
    // addressLen --> the len in bytes of struct address
    int opt, addressLen;

public:
    Server(){
        opt = 1;
        addressLen = sizeof(address); // get bytes len of address and assign
    }

    // Function that try to create a new server socket and configurate it
    int createServer(){
        // Try to create the file descriptor of server socket
        if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            // Error message if fail
            cout << "Socket failed" << endl;
            exit(EXIT_FAILURE);
        }

        // Try to forcefully set the port socket to 8080
        if (setsockopt(serverSocket, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt))) {
            // Error message if fail
            cout << "Error trying to set the socket parameters" << endl;
            exit(EXIT_FAILURE);
        }

        // Set the int attributes of address struct
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Try to export the configuration of address struct and bind it to the socket
        if (bind(serverSocket, (struct sockaddr*)&address,sizeof(address)) < 0) {
            // Error message if fail
            cout << "Bind failed" << endl;
            exit(EXIT_FAILURE);
        }
        cout << ">>> Server successfully turned on <<<" << endl;

        // Try to put the server waiting for clients
        if (listen(serverSocket, 3) < 0) {
            // Error message if fail
            cout << "Listen failed" << endl;
            exit(EXIT_FAILURE);
        }
        cout << ">>> Server waiting for connections <<<" << endl;

        // Accept the solicitude of a new client and create a specific socket for him
        if ((newSocket = accept(serverSocket, (struct sockaddr*)&address,(socklen_t*)&addressLen)) < 0) {
            // Error message if fail
            cout <<"Accept failed" << endl;
            exit(EXIT_FAILURE);
        }
        connection = true; // set in active the connection between server and client
        cout << ">> New client has connected <<" << endl;

        return 0;
    }

    // Function that get a text line, wrote by the user in the console and then, send it through the socket
    int sendMessage(){
        cout << ">>> Write your message <<<" << endl;
        string message;
        getline(cin, message); // get the text line wrote in console and save it in variable message
        char* hello = (char*) message.c_str(); // create a charPtr with the text data
        send(newSocket, hello, strlen(hello), 0); // send the text through the socket to client
        return 0;
    }

    // Function that hear the data from client and save it
    int hearMessage(){
        while (connection){ // if the connection between client and server still active
            char *buffer = new char[1024]; // create a buffer where the text receive from sockets is safe
            read(newSocket, buffer, 1024); // read the data and save it in buffer
            cout << "Client: " << buffer << endl; // print the message in console
            delete[] buffer;
            sendMessage(); // Start answer message protocol
        }
        return 0;
    }

    // Function that close the socket with the client
    int closeSocket(){
        // Closing the connected socket
        close(newSocket);
        cout << "The client socket was successfully closed" << endl;
        return 0;
    }

    // Function that turn off the server socket
    int closeServer(){
        // Closing the listening socket
        shutdown(serverSocket, SHUT_RDWR);
        cout << "The server was successfully turned off" << endl;
        return 0;
    }
};

// The main function create a new instance of server and initialize it
int main(){
    Server server;
    server.createServer();
    server.hearMessage();
    server.closeSocket();
    server.closeServer();
    return 0;
}
