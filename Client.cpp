#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>

#define PORT 8080

#define GAMMA 1
#define GAUSSIAN 2
#define BRIGHTNESS 3
#define GRAYSCALE 4

#define SEND_WIDTH 5
#define SEND_HEIGHT 6
#define SEND_FILTER 7
#define SEND_BRIGHT 8
#define SEND_IMAGE 9

#define CLOSE_SOCKET 10


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

    // image and filterImage --> place where the image is saved
    Mat image;
    Mat filterImage;

public:
    // imgSize --> size of bits of the image
    // imgWidth --> width in pixels of the image
    // imgHeight --> height in pixels of the image
    int imgSize, imgWidth, imgHeight;
    int filter, brightness;

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

        filterProtocol(filter);
        return 0;
    }

    int filterProtocol(int filter_num){
        if (filter_num == BRIGHTNESS){
            sendNumber(SEND_BRIGHT);
            sendNumber(brightness);
        }

        sendNumber(SEND_FILTER);
        sendNumber(filter);

        sendNumber(SEND_WIDTH);
        sendNumber(imgWidth);

        sendNumber(SEND_HEIGHT);
        sendNumber(imgHeight);

        sendNumber(SEND_IMAGE);
        sendImage();

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
    int sendImage(){
        int bytes = 0;
        bytes = send(newSocket, image.data, imgSize, 0); // send the image through the socket to client
        if (bytes < 0){ // Error code
            cout << "Error writing to socket" << endl;
            closeSocket();
        }
        return 0;
    }

    // Function that hear the image from server and save it
    int hearImage(){
        int bytes;
        filterImage = Mat::zeros(imgHeight, imgWidth, CV_8UC3); // initialize the mat instance with the size of image

        uchar sockData[imgSize]; // Buffer where the pixels of the image are saved

        for (int i = 0; i < imgSize; i+=bytes) {
            if ((bytes = recv(newSocket, sockData + i, imgSize - i, 0)) == -1){ // read the data and save it in buffer
                cout << "Receive failed" << endl;
                return -1;
            }
        }
        int ptr = 0;
        cout << imgHeight << endl << imgWidth << endl;
        for (int i = 0; i < imgHeight; ++i) { // Get the color information in the buffer and apply it to Mat instance
            for (int j = 0; j < imgWidth; ++j) {
                if (filter == GRAYSCALE) {
                    filterImage.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr],sockData[ptr],sockData[ptr]);
                    ptr += 1;
                } else{
                    filterImage.at<cv::Vec3b>(i, j) = cv::Vec3b(sockData[ptr + 0], sockData[ptr + 1],sockData[ptr + 2]);
                    ptr += 3;
                }
            }
        }

        // Show the image in screen
        namedWindow("Image received by Client (Image with Filter)", WINDOW_AUTOSIZE);
        imshow("Image received by Client (Image with Filter)", filterImage);
        waitKey(0);

        return 0;
    }

    // Function that close the socket cnnection with the server
    int closeSocket(){
        sendNumber(CLOSE_SOCKET);
        close(clientSocket);
        cout << "The client socket was successfully closed" << endl;
        return 0;
    }
};

// The main function create a new instance of server and initialize it
int main(){

    Client client("imagen3.jpg");

    client.conectServer();

    client.chooseFilter();

    client.hearImage();

    client.closeSocket();

    return 0;
}

