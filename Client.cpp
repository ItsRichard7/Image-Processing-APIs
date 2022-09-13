#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

using namespace std;

class Client{

private:
    // connection --> boolean that indicates if the client and server are actually texting
    // newSocket --> save the configuration of the socket
    // clientSocket --> communication channel with server
    int connection, newSocket, clientSocket;

    // address --> struct with int attributes for configure later the parameters of socket
    struct sockaddr_in serverAdress;

public:
    Client(){
        newSocket = 0;
    }

    // Function that try to connect a new socket with a server in a specific port and ip
    int conectServer(){
        // Try to create the file descriptor of socket
        if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "Socket creation error" << endl;
            return -1;
        }

        // Set the int attributes of address struct
        serverAdress.sin_family = AF_INET;
        serverAdress.sin_port = htons(PORT);

        // Convert addresses from text to binary
        if (inet_pton(AF_INET, "127.0.0.1", &serverAdress.sin_addr)<= 0) {
            cout << "Invalid address: Address not supported" << endl;
            return -1;
        }

        // Try to start a connection with the server
        if ((clientSocket = connect(newSocket, (struct sockaddr*)&serverAdress,sizeof(serverAdress)))< 0) {
            cout << "Connection Failed" << endl;
            return -1;
        }
        connection = true; // set in active the connection between server and client
        cout << ">>> Connection with server successfully correct <<<" << endl;
        return 0;
    }

    // Function that get a text line, wrote by the user in the console and then, send it through the socket
    int sendMessage(){
        cout << ">>> Write your message <<<" << endl;
        string message;
        getline(cin, message); // get the text line wrote in console and save it in variable message
        char* data = (char*) message.c_str(); // create a charPtr with the text data
        send(newSocket, data, strlen(data), 0); // send the text through the socket to client
        return 0;
    }

    // Function that hear the data from client and save it
    int hearMessages(){
        while (connection){ // if the connection between client and server still active
            char *buffer = new char[1024]; // create a buffer where the text receive from sockets is safe
            read(newSocket, buffer, 1024); // read the data and save it in buffer
            cout << "Server: " << buffer << endl; // print the message in console
            delete[] buffer;
            sendMessage(); // Start answer message protocol
        }
        return 0;
    }

    // Function that close the socket cnnection with the server
    int closeSocket(){
        close(clientSocket);
        cout << "The client socket was successfully closed" << endl;
        return 0;
    }
};

// The main function create a new instance of server and initialize it
int main(){
    Client client;
    client.conectServer();
    client.sendMessage();
    client.hearMessages();
    client.closeSocket();
    return 0;
}

