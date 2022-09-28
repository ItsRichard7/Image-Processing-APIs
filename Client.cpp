#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include "ImagePagination.cpp"

#define PORT 8080

#define GAMMA 1
#define GAUSSIAN 2
#define BRIGHTNESS 3
#define GRAYSCALE 4

#define SEND_WIDTH 5
#define SEND_HEIGHT 6
#define SEND_BLOCK_WIDTH 7
#define SEND_BLOCK_HEIGHT 8
#define SEND_FILTER 9
#define SEND_BRIGHT 10
#define SEND_GAMMA 11
#define SEND_IMAGE 12

#define APPLY_FILTER 13

#define CLOSE_SOCKET 14


using namespace std;
using namespace cv;

class Client{

private:
    // connection --> boolean that indicates if the client and server are actually texting
    // newSocket --> save the configuration of the socket
    // clientSocket --> communication channel with server
    int newSocket, clientSocket;

    // address --> struct with int attributes for configure later the parameters of socket
    struct sockaddr_in serverAdress;
    int imgSize, blockWidth, blockHeight;
    ImageHandler* image;

public:
    int filter, brightness, gamma;

    Client(string imgPath){
        newSocket = 0;
        image = new ImageHandler(imgPath);
        image->separateImage();
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
        cout << ">>> Connection with server successfully correct <<<" << endl;
        return 0;
    }

    int chooseFilter(){
        string message;
        int answer;

        cout << ">>> Type the number of the filter do tou want to apply to your image <<<" << endl;
        cout << "1) Gamma Filter  2) Gaussian Filter  3) Brightness Control  4) Gray Scale" << endl;

        getline(cin, message);
        answer = stoi(message);

        filter = answer;

        if (filter == BRIGHTNESS){
            cout << ">>> In a scale of 0 to 300 (150 its the same bright) how much brightness do you want to apply to your image <<<" << endl;
            string bright_message;
            int bright_int;
            getline(cin, bright_message);
            bright_int = stoi(bright_message);
            brightness = bright_int;
        }

        if (filter == GAMMA) {
            cout << ">>> In a scale of 0 to 3 define the gamma value you want to apply to your image <<<" << endl;
            string gamma_message;
            int gamma_int;
            getline(cin, gamma_message);
            gamma_int = stoi(gamma_message);
            brightness = gamma_int;
        }

        filterProtocol(filter);
        return 0;
    }

    int filterProtocol(int filter_num){
        if (filter_num == BRIGHTNESS){
            sendNumber(SEND_BRIGHT);
            sendNumber(brightness);
        }
        if (filter_num == GAMMA){
            sendNumber(SEND_GAMMA);
            sendNumber(gamma);
        }

        sendNumber(SEND_WIDTH);
        sendNumber(image->image.cols);

        sendNumber(SEND_HEIGHT);
        sendNumber(image->image.rows);

        sendNumber(SEND_FILTER);
        sendNumber(filter);

        for (int i = 0; i < image->blocks.size(); ++i) {
            blockWidth = image->blocks[i].cols;
            blockHeight = image->blocks[i].rows;

            sendNumber(SEND_BLOCK_WIDTH);
            sendNumber(blockWidth);

            sendNumber(SEND_BLOCK_HEIGHT);
            sendNumber(blockHeight);

            sendNumber(SEND_IMAGE);
            sendImage(image->blocks[i]);
        }

        sendNumber(APPLY_FILTER);

        for (int i = 0; i < image->blocks.size(); ++i) {
            blockWidth = image->blocks[i].cols;
            blockHeight = image->blocks[i].rows;
            hearImage();
        }
        image->remakeImage();
        image->showImage("Image received by Client (Image with Filter)",image->filterImage);
        return 0;
    }


    // Send int values
    int sendNumber(int value){
        int num = value;
        int* numPtr = &num;
        int n = send(newSocket,numPtr, sizeof(int), 0);
        if (n < 0){
            cout << "Error writing to socket" << endl;
            closeSocket();
        }
        return 0;
    }

    // Function that send image through the socket
    int sendImage(Mat Image){
        int bytes = 0;
        imgSize = Image.total()*Image.elemSize();
        bytes = send(newSocket, Image.data, imgSize, 0); // send the image through the socket to client
        if (bytes < 0){ // Error code
            cout << "Error writing to socket" << endl;
            closeSocket();
        }
        return 0;
    }

    // Function that hear the image from server and save it
    int hearImage(){
        int bytes;
        Mat filterBlock = Mat::zeros(blockHeight, blockWidth, CV_8UC3); // initialize the mat instance with the size of image

        imgSize = filterBlock.total()*filterBlock.elemSize(); // get the bit size of image
        uchar sockData[imgSize]; // Buffer where the pixels of the image are saved

        for (int i = 0; i < imgSize; i+=bytes) {
            if ((bytes = recv(newSocket, sockData + i, imgSize - i, 0)) == -1){ // read the data and save it in buffer
                cout << "Receive failed" << endl;
                return -1;
            }
        }
        int ptr = 0;
        for (int i = 0; i < blockHeight; ++i) { // Get the color information in the buffer and apply it to Mat instance
            for (int j = 0; j < blockWidth; ++j) {
                if (filter == GRAYSCALE) {
                    filterBlock.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr],sockData[ptr],sockData[ptr]);
                    ptr += 1;
                } else{
                    filterBlock.at<cv::Vec3b>(i, j) = cv::Vec3b(sockData[ptr + 0], sockData[ptr + 1],sockData[ptr + 2]);
                    ptr += 3;
                }
            }
        }

        image->filterBlocks.push_back(filterBlock);
        return 0;
    }

    // Function that close the socket connection with the server
    int closeSocket(){
        sendNumber(CLOSE_SOCKET);
        close(clientSocket);
        cout << ">>> The client socket was successfully closed <<<" << endl;
        return 0;
    }
};

// The main function create a new instance of server and initialize it
int main(){
    string imgPath;
    cout << ">>> Type the path of the image you want to edit <<<" << endl;
    getline(cin, imgPath);
    Client client(imgPath);
    client.conectServer();
    client.chooseFilter();
    client.closeSocket();
    return 0;
}

