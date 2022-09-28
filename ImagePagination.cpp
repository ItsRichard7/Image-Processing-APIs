#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class ImageHandler{
public:
    int widthBlock, heightBlock;

    // imgSize --> size of bits of the image
    // imgWidth --> width in pixels of the image
    // imgHeight --> height in pixels of the image
    int imgSize, imgWidth, imgHeight;

    int horizontalBlocks,verticalBlocks;

    int lastBlockW, lastBlockH;

    string imagepath;
    Mat image;
    Mat filterImage;

    vector<Mat> blocks;
    vector<Mat> filterBlocks;

    ImageHandler(string path){
        widthBlock = 100;
        heightBlock = 100;
        imagepath = path;
        image = imread(path, 1);
        if (!image.data){
            cout << "No image data" << endl;
            exit(0);
        }
        imgSize = image.total()*image.elemSize();
        imgWidth = image.cols;
        imgHeight = image.rows;
        horizontalBlocks = (int) (imgWidth / widthBlock);
        cout << horizontalBlocks << endl;
        verticalBlocks = (int) (imgHeight / heightBlock);
        cout << verticalBlocks << endl;
        lastBlockW = imgWidth % widthBlock;
        cout << lastBlockW << endl;
        lastBlockH = imgHeight % heightBlock;
        cout << lastBlockH << endl;

        if (lastBlockW != 0) horizontalBlocks++;
        if (lastBlockH != 0) verticalBlocks++;
        filterImage = Mat::zeros(imgHeight, imgWidth, CV_8UC3);
    }

    int showImage(string winName, Mat image){
        namedWindow(winName, WINDOW_AUTOSIZE);
        imshow(winName, image);
        waitKey(0);
        return 0;
    }

    int showVector(){
        for (int i = 0; i < blocks.size(); ++i) {
            showImage(("Block #" + to_string(i + 1)), blocks[i]);
        }
        return 0;
    }

    int separateImage(){
        int xpos = 0, ypos = 0, bwSize, bhSize;
        for (int i = 0; i < verticalBlocks; ++i) {
            if (i == (verticalBlocks - 1) && lastBlockH != 0) bhSize = lastBlockH;
            else bhSize = heightBlock;
            xpos = 0;
            for (int j = 0; j < horizontalBlocks; ++j) {
                if (j == (horizontalBlocks - 1) && lastBlockW != 0) bwSize = lastBlockW;
                else bwSize = widthBlock;
                blocks.push_back(image(Rect(xpos, ypos, bwSize, bhSize)).clone());
                xpos += widthBlock;
            }
            ypos += heightBlock;
        }
        return 0;
    }

    int remakeImage(){
        int xpos, ypos, tempx, tempy;
        for (int i = 0; i < filterBlocks.size(); ++i) {
            tempy = (int) i / horizontalBlocks;
            ypos = heightBlock * tempy;
            for (int j = 0; j < blocks[i].rows; ++j) {
                tempx = i % horizontalBlocks;
                xpos = widthBlock * tempx;
                for (int k = 0; k < filterBlocks[i].cols; ++k) {
                    cv::Vec3b pixel = filterBlocks[i].at<cv::Vec3b>(j,k);
                    filterImage.at<cv::Vec3b>(ypos, xpos) = pixel;
                    xpos++;
                }
                ypos++;
            }
        }
    }
};

int main(){
    ImageHandler imageHandler("imagen1.jpg");
    imageHandler.showImage("Image loaded by user", imageHandler.image);
    imageHandler.separateImage();
    imageHandler.filterBlocks = imageHandler.blocks;
    imageHandler.remakeImage();
    imageHandler.showImage("Remake image from blocks", imageHandler.filterImage);
    return 0;
}