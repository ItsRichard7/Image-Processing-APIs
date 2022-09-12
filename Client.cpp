#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

using namespace std;

class Client{

private:
    int connection, newSocket, clientSocket;
    struct sockaddr_in serverAdress;

public:
    Client(){
        connection = true;
        newSocket = 0;
    }

    int conectServer(){
        if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "Socket creation error" << endl;
            return -1;
        }

        serverAdress.sin_family = AF_INET;
        serverAdress.sin_port = htons(PORT);

        // Convert IPv4 and IPv6 addresses from text to binary
        // form
        if (inet_pton(AF_INET, "127.0.0.1", &serverAdress.sin_addr)<= 0) {
            cout << "Invalid address: Address not supported" << endl;
            return -1;
        }

        if ((clientSocket = connect(newSocket, (struct sockaddr*)&serverAdress,sizeof(serverAdress)))< 0) {
            cout << "Connection Failed" << endl;
            return -1;
        }
        cout << "The connection with server was successfully correct" << endl;
        return 0;
    }

    int sendMessage(){
        cout << "Write your message:" << endl;
        string message;
        getline(cin, message);
        char* data = (char*) message.c_str();
        send(newSocket, data, strlen(data), 0);
        return 0;
    }

    int hearMessages(){
        while (connection){
            char *buffer = new char[1024];
            read(newSocket, buffer, 1024);
            cout << "Server: " << buffer << endl;
            int value;
            try {
                sscanf(buffer, "%d", value);
                if (value == 1){
                    connection = false;
                }
            } catch (...){}
            delete[] buffer;
            sendMessage();
        }
        return 0;
    }

    int closeSocket(){
        close(clientSocket);
        cout << "The client socket was successfully closed" << endl;
        return 0;
    }
};

int main(){
    Client client;
    client.conectServer();
    client.sendMessage();
    client.hearMessages();
    client.closeSocket();
    return 0;
}

