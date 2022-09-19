#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#define PORT 8080

using namespace std;
using namespace cv;

class Client{

private:
    // connection --> boolean that indicates if the client and server are actually texting
    // newSocket --> save the configuration of the socket
    // clientSocket --> communication channel with server
    int connection, newSocket, clientSocket;

    // address --> struct with int attributes for configure later the parameters of socket
    struct sockaddr_in serverAdress;

    // image --> place where the image is saved
    Mat image;
    Mat filterImage;
public:
    int imgSize, imgWidth, imgHeight;

    Client(string imgPath){
        newSocket = 0;
        image = imread(imgPath, 1);
        if (!image.data){
            cout << "No image data" << endl;
        }
        imgSize = image.total()*image.elemSize();
        imgWidth = image.cols;
        imgHeight = image.rows;
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

    int sendSize(int* value){
        int n = send(newSocket,value, sizeof(int), 0);
        if (n < 0){
            cout << "Error writing to socket" << endl;
            closeSocket();
        }
        return 0;
    }

    // Function that get a text line, wrote by the user in the console and then, send it through the socket
    int sendImage(){
        int bytes = 0;
        bytes = send(newSocket, image.data, imgSize, 0); // send the image through the socket to client
        if (bytes < 0){
            cout << "Error writing to socket" << endl;
            closeSocket();
        }
        return 0;
    }

    // Function that hear the data from client and save it
    int hearImage(){
        int bytes;
        filterImage = Mat::zeros(imgHeight, imgWidth, CV_8UC3);
        imgSize = filterImage.total() * filterImage.elemSize();
        uchar sockData[imgSize];

        for (int i = 0; i < imgSize; i+=bytes) {
            if ((bytes = recv(newSocket, sockData + i, imgSize - i, 0)) == -1){ // read the data and save it in buffer
                cout << "Receive failed" << endl;
                return -1;
            }
        }

        int ptr = 0;
        for (int i = 0; i < filterImage.rows; ++i) {
            for (int j = 0; j < filterImage.cols; ++j) {
                filterImage.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr + 0], sockData[ptr + 1], sockData[ptr + 2]);
                ptr += 3;
            }
        }

        namedWindow("Client", WINDOW_AUTOSIZE);
        imshow("Client", filterImage);
        waitKey(0);
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
    //Client client("bob.png");
    Client client("banda.jpg");
    client.conectServer();

    client.sendSize(&client.imgWidth);
    client.sendSize(&client.imgHeight);
    client.sendImage();

    client.hearImage();

    client.closeSocket();
    return 0;
}

