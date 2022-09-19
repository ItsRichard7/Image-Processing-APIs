//
// Created by cruz on 19/09/22.
//
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Processing_APIS.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core.hpp>

using namespace cv;

Mat Processing_APIS::GaussianFilter(Mat img, Mat img2,int i ) {
    int j= 0;
    GaussianBlur(img, img2,Size(15,15), 0);

    while (j < i){
        GaussianBlur(img2, img2,Size(15,15), 0);
        j++;
    }
    namedWindow("Gaussian2",0);
    imshow("Gaussian2",img2);
    return img2;
}

Mat Processing_APIS::GammaFilter(Mat img, Mat img2, int gamma) {


    double c, r_min,r_max;
    minMaxLoc(img, &r_min,&r_max);
    c= 255/pow(r_max,gamma);
    img.convertTo(img,CV_32F);
    pow(img, gamma, img2);
    img2= c*img2;
    img2.convertTo(img2,CV_8UC1);

    return img2;
}

Mat Processing_APIS::BrightnessControlFilter(Mat img, int bright, int contrast) {
    Mat img2;
    img.convertTo(img2,-1,contrast,bright-150);
    return img2;
}

Mat Processing_APIS::GrayScaleFilter(Mat img, Mat img2) {
    cvtColor(img,img2, COLOR_BGR2GRAY);

    return img2;
}
