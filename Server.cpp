#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

using namespace std;

class Server{

private:
    int connection, serverSocket, newSocket;
    struct sockaddr_in address;
    int opt, addressLen;

public:
    Server(){
        opt = 1;
        addressLen = sizeof(address);
    }
    int createServer(){
        // Creating socket file descriptor
        if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            cout << "Socket failed" << endl;
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port 8080
        if (setsockopt(serverSocket, SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt))) {
            cout << "Error trying to set the socket parameters" << endl;
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(serverSocket, (struct sockaddr*)&address,sizeof(address)) < 0) {
            cout << "Bind failed" << endl;
            exit(EXIT_FAILURE);
        }
        cout << "The server was successfully turned on" << endl;

        if (listen(serverSocket, 3) < 0) {
            cout << "Listen failed" << endl;
            exit(EXIT_FAILURE);
        }
        cout << "...Server waiting for connections..." << endl;

        if ((newSocket = accept(serverSocket, (struct sockaddr*)&address,(socklen_t*)&addressLen)) < 0) {
            cout <<"Accept failed" << endl;
            exit(EXIT_FAILURE);
        }
        cout << "New client has connected" << endl;

        return 0;
    }

    int sendMessage(){
        if (connection){
            cout << "Write your message:" << endl;
            string message;
            getline(cin, message);
            char* hello = (char*) message.c_str();
            send(newSocket, hello, strlen(hello), 0);
            return 0;
        } else{
            string message = "1";
            char* data = (char*) message.c_str();
            send(newSocket, data, strlen(data), 0);
            return 0;
        }
    }

    int hearMessage(){
        while (connection){
            char *buffer = new char[1024];
            read(newSocket, buffer, 1024);
            cout << "Client: " << buffer << endl;
            try {
                int value;
                sscanf(buffer, "%d", value);
                if (value == 1){
                    connection = false;
                    return 0;
                }
            } catch (...){}
            delete[] buffer;
            sendMessage();
        }
        return 0;
    }

    int closeSocket(){
        // closing the connected socket
        close(newSocket);
        cout << "The client socket was successfully closed" << endl;
        return 0;
    }

    int closeServer(){
        // closing the listening socket
        shutdown(serverSocket, SHUT_RDWR);
        cout << "The server was successfully turned off" << endl;
        return 0;
    }
};

int main(){
    Server server;
    server.createServer();
    server.hearMessage();
    server.sendMessage();
    server.closeSocket();
    server.closeServer();
    return 0;
}
