#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class ImageHandler{
public:
    // imgSize --> size of bits of the image
    // imgWidth and imgHeight --> width and height in pixels of the image
    int imgWidth, imgHeight;

    // String with the local path of the image
    string imagepath;

    // image and filterImage --> place where the image is saved
    Mat image, filterImage;

    // size in pixels of any block, also the last block to
    int widthBlock, heightBlock, lastBlockW, lastBlockH;

    // number of blocks of any row and colum
    int horizontalBlocks,verticalBlocks;

    // Dinamic data structure that contends all the blocks
    vector<Mat> blocks,filterBlocks;

    // Constructor for server side (the server do not have the image when the program starts)
    ImageHandler(){
        // Here you set the size of the blocks do you want
        widthBlock = 100;
        heightBlock = 100;
    }

    // Constructor for client side (have the image when the program starts)
    ImageHandler(string path){
        // Here you set the size of the blocks do you want
        widthBlock = 100;
        heightBlock = 100;

        // Read the image given
        imagepath = path;
        image = imread(path, 1);
        if (!image.data){ // print error message if the image do not found
            cout << "No image data" << endl;
            exit(0);
        }
        // Set the dimensions of the image
        imgWidth = image.cols;
        imgHeight = image.rows;

        setParameters();
    }

    // Function that defines some parameters of class (kind of builder pattern)
    int setParameters(){
        // Get the number of blocks of columns and rows
        horizontalBlocks = (int) (imgWidth / widthBlock);
        verticalBlocks = (int) (imgHeight / heightBlock);

        // Get the size of pixels of the last block
        lastBlockW = imgWidth % widthBlock;
        lastBlockH = imgHeight % heightBlock;

        // If the image have blocks with different sizes (the last blocks) we need to aggregate it to the number of blocks
        if (lastBlockW != 0) horizontalBlocks++;
        if (lastBlockH != 0) verticalBlocks++;

        // initialize the filter image for later give it color
        filterImage = Mat::zeros(imgHeight, imgWidth, CV_8UC3);
        return 0;
    }

    // Function that shows a specific image on the screen
    int showImage(string winName, Mat image){
        namedWindow(winName, WINDOW_AUTOSIZE);
        imshow(winName, image);
        waitKey(0);
        return 0;
    }

    // Auxiliar function to show all the blocks in vector
    int showVector(){
        for (int i = 0; i < blocks.size(); ++i) {
            showImage(("Block #" + to_string(i + 1)), blocks[i]);
        }
        return 0;
    }


    int separateImage(){
        int xpos = 0, ypos = 0, bwSize, bhSize;
        for (int i = 0; i < verticalBlocks; ++i) { // Go through the number of vertical blocks
            // Define the size of the y-axis cut
            if (i == (verticalBlocks - 1) && lastBlockH != 0) bhSize = lastBlockH;
            else bhSize = heightBlock;
            xpos = 0;
            for (int j = 0; j < horizontalBlocks; ++j) { // Go through the number of horizontal blocks
                // Define the size of the x-axis cut
                if (j == (horizontalBlocks - 1) && lastBlockW != 0) bwSize = lastBlockW;
                else bwSize = widthBlock;

                blocks.push_back(image(Rect(xpos, ypos, bwSize, bhSize)).clone()); // make the block and save it in vector
                xpos += widthBlock;
            }
            ypos += heightBlock;
        }
        return 0;
    }

    int remakeImage(){
        int xpos, ypos, tempx, tempy;
        for (int i = 0; i < filterBlocks.size(); ++i) { // go through all the blocks in the vector
            tempy = (int) i / horizontalBlocks;
            ypos = heightBlock * tempy;
            for (int j = 0; j < filterBlocks[i].rows; ++j) { // go through the rows of the block
                tempx = i % horizontalBlocks;
                xpos = widthBlock * tempx;
                for (int k = 0; k < filterBlocks[i].cols; ++k) { // go through the columns of the block
                    cv::Vec3b pixel = filterBlocks[i].at<cv::Vec3b>(j,k); // save the color of the block pixel
                    filterImage.at<cv::Vec3b>(ypos, xpos) = pixel; // assign the color to filter image
                    xpos++;
                }
                ypos++;
            }
        }
        return 0;
    }
};