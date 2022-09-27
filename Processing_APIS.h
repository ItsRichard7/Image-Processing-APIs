//
// Created by cruz on 19/09/22.
//
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

class Processing_APIS{

public:
    static int gaussian_blur(Mat* img, Mat* img2);
    static int gamma_correction(Mat* img, Mat* img2);
    static int bright_control(Mat* img, Mat* img2, int bright);
    static int gray_scale(Mat* img, Mat* img2);

private:

};


#ifndef IMAGE_PROCESSING_PROCESSING_APIS_H
#define IMAGE_PROCESSING_PROCESSING_APIS_H

#endif //IMAGE_PROCESSING_PROCESSING_APIS_H
