//
// Created by cruz on 19/09/22.
//
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

class Processing_APIS{

public:
    static Mat GaussianFilter(Mat img, Mat img2, int i);
    static Mat GammaFilter(Mat img, Mat img2, int gamma);
    static Mat BrightnessControlFilter(Mat img, int bright, int contrast);
    static Mat GrayScaleFilter(Mat img, Mat img2);

private:

};


#ifndef IMAGE_PROCESSING_PROCESSING_APIS_H
#define IMAGE_PROCESSING_PROCESSING_APIS_H

#endif //IMAGE_PROCESSING_PROCESSING_APIS_H
