#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include "Processing_APIS.h"

#define PORT 8080

#define GAMMA 1
#define GAUSSIAN 2
#define BRIGHTNESS 3
#define GRAYSCALE 4

#define RECEIVE_WIDTH 5
#define RECEIVE_HEIGHT 6
#define RECEIVE_FILTER 7
#define RECEIVE_BRIGHT 8
#define RECEIVE_IMAGE 9

#define CLOSE_SOCKET 10

using namespace std;
using namespace cv;


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

    // image and filterImage --> Both save the client image. The original and the filter one
    Mat image;
    Mat* image_ptr;
    Mat filterImage;
    Mat* filterImage_ptr;



    // imgSize --> size of bits of the image
    // imgWidth --> width in pixels of the image
    // imgHeight --> height in pixels of the image
    // data --> number control variable
    int imgFilter, imgSize, imgWidth, imgHeight, imgBright;

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

    int hearNum(){
        int value;
        int n = recv(newSocket, &value, sizeof(value), 0);
        if (n < 0){
            cout << "Receive failed" << endl;
            return -1;
        }
        return value;
    }

    int hearMessages(){
        while (connection){
            int value = hearNum();
            if (value == RECEIVE_FILTER) setFilter();
            if (value == RECEIVE_BRIGHT) setBright();
            if (value == RECEIVE_WIDTH) setWidth();
            if (value == RECEIVE_HEIGHT) setHeight();
            if (value == RECEIVE_IMAGE){
                hearImage();
                applyFilter();
            }
            if (value == CLOSE_SOCKET) connection = false;
        }
        return 0;
    }

    int setFilter(){
        int value = hearNum();
        imgFilter = value;
    }

    int setBright(){
        int value = hearNum();
        imgBright = value;
    }

    int setWidth(){
        int value = hearNum();
        imgWidth = value;
    }

    int setHeight(){
        int value = hearNum();
        imgHeight = value;
    }

    // Function that hear the image from client and save it
    int hearImage(){
        int bytes;
        image = Mat::zeros(imgHeight, imgWidth, CV_8UC3); // initialize the mat instance with the size of image
        imgSize = image.total()*image.elemSize(); // get the bit size of image
        uchar sockData[imgSize]; // Buffer where the pixels of the image are saved

        for (int i = 0; i < imgSize; i+=bytes) {
            if ((bytes = recv(newSocket, sockData + i, imgSize - i, 0)) == -1){ // read the data and save it in buffer
                cout << "Receive failed" << endl;
                return -1;
            }
        }

        int ptr = 0;

        for (int i = 0; i < image.rows; ++i) { // Get the color information in the buffer and apply it to Mat instance
            for (int j = 0; j < image.cols; ++j) {
                image.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr + 0], sockData[ptr + 1], sockData[ptr + 2]);
                ptr += 3;
            }
        }

        // Show the image in screen
        namedWindow("Image received by Server (Original Image)", WINDOW_AUTOSIZE);
        imshow("Image received by Server (Original Image)", image);
        waitKey(0);

        return 0;
    }

    int applyFilter(){
        filterImage = image;
        image_ptr= &image;
        filterImage_ptr= &filterImage;
        if (imgFilter == GAMMA) Processing_APIS::gamma_correction(image_ptr, filterImage_ptr);

        if (imgFilter == GAUSSIAN) Processing_APIS::gaussian_blur(image_ptr, filterImage_ptr);

        if (imgFilter == GRAYSCALE) Processing_APIS::gray_scale(image_ptr,filterImage_ptr);

        if (imgFilter == BRIGHTNESS) Processing_APIS::bright_control(image_ptr,filterImage_ptr, imgBright);

        sendImage();
    }

    // Function that send image through the socket
    int sendImage(){
        int bytes = 0;
        bytes = send(newSocket, filterImage.data, imgSize, 0); // send the image through the socket to client
        if (bytes < 0){ // Error code
            cout << "Error writing to socket" << endl;
            closeSocket();
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

    server.hearMessages();

    server.closeSocket();

    server.closeServer();

    return 0;
}
