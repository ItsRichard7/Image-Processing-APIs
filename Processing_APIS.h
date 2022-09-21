//
// Created by cruz on 19/09/22.
//
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#ifndef IMAGE_PROCESSING_PROCESSING_APIS_H

using namespace cv;

class Processing_APIS{

public:
    static Mat gaussian_blur(Mat image);
    static Mat gamma_correction(Mat image);
    static Mat bright_control(Mat image, int bright);
    static Mat gray_scale(Mat image);
};

#define IMAGE_PROCESSING_PROCESSING_APIS_H

#endif //IMAGE_PROCESSING_PROCESSING_APIS_H
