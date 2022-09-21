//
// Created by cruz on 19/09/22.
//

#include <opencv2/imgproc.hpp>
#include "Processing_APIS.h"
#include <opencv2/core.hpp>

using namespace cv;

Mat Processing_APIS::gaussian_blur(Mat image) {
    Mat image_out;
    GaussianBlur(image, image_out,Size(15,15), 0);
    return image_out;
}

Mat Processing_APIS::gamma_correction(Mat image) {

    Mat image_out;
    int gamma = 2;
    double c, r_min,r_max;
    minMaxLoc(image, &r_min,&r_max);
    c= 255/pow(r_max,gamma);
    image.convertTo(image,CV_32F);
    pow(image, gamma, image_out);
    image_out = c * image_out;
    image_out.convertTo(image_out,CV_8UC1);
    return image_out;
}

Mat Processing_APIS::bright_control(Mat image, int bright) {
    Mat image_out;
    image.convertTo(image_out,-1, 1 ,bright-150);
    return image_out;
}

Mat Processing_APIS::gray_scale(Mat image) {
    Mat image_out;
    cvtColor(image,image_out, COLOR_RGB2GRAY);
    return image_out;
}
