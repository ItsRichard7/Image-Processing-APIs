#include <iostream>
#include <opencv2/opencv.hpp>
#include "Processing_APIS.h"
#include "gtest/gtest.h"

using namespace cv;
using namespace std;



TEST(Processing_APIS_Test, try_Gaussian){
    Mat img= imread("/home/cruz/Documents/Image-Processing-APIs/Pruebas/Imagen1.jpg");
    Mat img2;
    Mat* img2_pntr;
    Mat* img_pntr;
    img_pntr=&img;
    img2_pntr=&img2;

    int z= Processing_APIS::gaussian_blur(img_pntr, img2_pntr);

    ASSERT_EQ(z,0);
};

TEST(Processing_APIS_Test, try_Gray){
    Mat img= imread("/home/cruz/Documents/Image-Processing-APIs/Pruebas/Imagen1.jpg");
    Mat img2;
    Mat* img2_pntr;
    Mat* img_pntr;
    img_pntr=&img;
    img2_pntr=&img2;

    int w= Processing_APIS ::gray_scale(img_pntr, img2_pntr);


    ASSERT_EQ(w,0);
};

TEST(Processing_APIS_Test, try_Bright){
    Mat img= imread("/home/cruz/Documents/Image-Processing-APIs/Pruebas/Imagen1.jpg");
    Mat img2;
    Mat* img2_pntr;
    Mat* img_pntr;
    img_pntr=&img;
    img2_pntr=&img2;


    int y= Processing_APIS::bright_control(img_pntr, img2_pntr,150);



    ASSERT_EQ(y,0);
};

TEST(Processing_APIS_Test, try_Gamma){
    Mat img= imread("/home/cruz/Documents/Image-Processing-APIs/Pruebas/Imagen1.jpg");
    Mat img2;
    Mat* img2_pntr;
    Mat* img_pntr;
    img_pntr=&img;
    img2_pntr=&img2;

    int x= Processing_APIS::gamma_correction(img_pntr, img2_pntr, 2);

    ASSERT_EQ(x,0);
};

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();

}
