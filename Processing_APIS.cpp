//
// Created by cruz on 19/09/22.
//
#include <opencv2/imgproc.hpp>
#include "Processing_APIS.h"
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;
int Processing_APIS::gaussian_blur(Mat* img, Mat* img2) {
    GaussianBlur(*img, *img2,Size(15,15), 0);
    return 0;
}

int Processing_APIS::gamma_correction(Mat* img, Mat* img2, int Gamma){
    double c, r_min,r_max;
    float gamma = float(Gamma);
    if (gamma == 1) gamma = 0.5;
    minMaxLoc(*img, &r_min,&r_max);
    c= 255/pow(r_max,gamma);
    img->convertTo(*img,CV_32F);
    pow(*img, gamma, *img2);
    *img2= c*(*img2);
    img2->convertTo(*img2,CV_8UC1);


    return 0;
}

int Processing_APIS::bright_control(Mat* img, Mat* img2, int bright) {

    img->convertTo(*img2,-1,1,bright-150);
    return 0;
}

int Processing_APIS::gray_scale(Mat* img, Mat* img2) {
    cvtColor(*img, *img2, COLOR_BGR2GRAY);
    return 0;
}