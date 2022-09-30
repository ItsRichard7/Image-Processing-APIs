//
// Created by cruz on 29/09/22.
//

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Processing_APIS.h"


using namespace cv;
using namespace std;


class Testing

{
public:
    int grayTest(Mat x, Mat y){

        for (int i = 0; i < x.cols; ++i) {
            for (int j = 0; j < y.rows; ++j) {
                if (x.at<Vec3b>(i,j) != y.at<Vec3b>(i,j))return -1;
            }
        }
        return 0;
    }
    int gaussianTest(Mat x, Mat y){
        for (int i = 0; i < x.cols; ++i) {
            for (int j = 0; j < y.rows; ++j) {
                if (x.at<Vec3b>(i,j) != y.at<Vec3b>(i,j))return -1;
                }
            }
            return 0;
    }

    int brightTest(Mat* x, Mat y){
        for (int i = 0; i < y.cols; ++i) {
            for (int j = 0; j < y.rows; ++j) {
                if (x->at<Vec3b>(i,j) != y.at<Vec3b>(i,j))return -1;
            }
        }
        return 0;
    }

    int gammaTest(Mat x, Mat y){
        for (int i = 0; i < x.cols; ++i) {
            for (int j = 0; j < y.rows; ++j) {
                if (x.at<Vec3b>(i,j) != y.at<Vec3b>(i,j))return -1;
            }
        }
        return 0;
    }
};


int main(){

    Testing testing;
    int estado;
    Mat img= imread("/home/cruz/Documents/Image-Processing-APIs/Pruebas/img_Prueba.jpg");
    Mat img_gray= imread("/home/cruz/Documents/Image-Processing-APIs/Pruebas/img_Gris.jpg");
    Mat img_gaussian=imread("/home/cruz/Documents/Image-Processing-APIs/Pruebas/img_Gaussian.jpg");
    Mat img_bright= imread("/home/cruz/Documents/Image-Processing-APIs/Pruebas/img_Bright.jpg");
    Mat img_gamma= imread("/home/cruz/Documents/Image-Processing-APIs/Pruebas/img_Gamma.jpg");
    


    Mat z;
    Mat* x_ptr;
    Mat* z_ptr;
    x_ptr=&img;
    z_ptr=&z;


    Processing_APIS::gaussian_blur(x_ptr,z_ptr);
    //imshow("GAussian", img_gaussian);
    //imwrite("/home/cruz/Documents/Image-Processing-APIs/Pruebas/img_Gaussian.jpg", img_gaussian);
    estado=testing.gaussianTest(z, img_gaussian);
    cout<<""<<endl;
    cout<<"Filtro GaussianBlur: "<<endl;
    if(estado==0){
        cout<<"Exito total!"<<endl;
    }else{cout<<"Fallo en el proceso"<<endl;}


    Processing_APIS::bright_control(x_ptr,z_ptr,200);
    estado=testing.brightTest(z_ptr,img_bright);
    cout<<""<<endl;
    cout<<"Filtro control de Brillo: "<<endl;
    if(estado==0){
        cout<<"Exito total!"<<endl;
    }else{cout<<"Fallo en el proceso"<<endl;}


    Processing_APIS::gamma_correction(x_ptr,z_ptr,2);
    //img_gamma=*z_ptr;

    estado=testing.gammaTest(z,img_gamma);
    cout<<""<<endl;
    cout<<"Filtro correcion Gamma: "<<endl;
    if(estado==0){
        cout<<"Exito total!"<<endl;
    }else{cout<<"Fallo en el proceso"<<endl;}

    Processing_APIS::gray_scale(x_ptr,z_ptr);
    //img_gray=*z_ptr;
    //imwrite("/home/cruz/Documents/Image-Processing-APIs/Pruebas/img_Gris.jpg", img_gray);
    estado=testing.grayTest(z, img_gray);
    cout<<""<<endl;
    cout<<"Filtro escala de grises: "<<endl;
    if(estado==0){
        cout<<"Exito total!"<<endl;
    }else{cout<<"Fallo en el proceso"<<endl;}

}





