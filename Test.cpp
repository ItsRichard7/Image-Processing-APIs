//
// Created by cruz on 29/09/22.
//

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
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
                if (x.at<Vec3b>(i,j) != y.at<Vec3b>(i,j)) return -1;
                }
            }
            return 0;
    }

    int brightTest(Mat x, Mat y){
        for (int i = 0; i < y.cols; ++i) {
            for (int j = 0; j < y.rows; ++j) {
                if (x.at<Vec3b>(i,j) != y.at<Vec3b>(i,j)) return -1;
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
    Mat img = imread("/home/itsrichard/Proyectos C++/Programadas/Proyecto I/Image-Processing-APIs/Pruebas/img_Prueba.jpg");
    Mat img_gray = imread("/home/itsrichard/Proyectos C++/Programadas/Proyecto I/Image-Processing-APIs/Pruebas/img_Gris.jpg");
    Mat img_gaussian =imread("/home/itsrichard/Proyectos C++/Programadas/Proyecto I/Image-Processing-APIs/Pruebas/img_Gaussian.jpg");
    Mat img_bright = imread("/home/itsrichard/Proyectos C++/Programadas/Proyecto I/Image-Processing-APIs/Pruebas/img_Bright.jpg");
    Mat img_gamma = imread("/home/itsrichard/Proyectos C++/Programadas/Proyecto I/Image-Processing-APIs/Pruebas/img_Gamma.jpg");

    Mat gray;
    Mat gaussian;
    Mat bright;
    Mat gamma;
    Mat* imgPtr = &img;
    Mat* grayPtr = &gray;
    Mat* gaussianPtr = &gaussian;
    Mat* brightPtr = &bright;
    Mat* gammaPtr = &gamma;

    Processing_APIS::gaussian_blur(imgPtr, gaussianPtr);
    estado = testing.gaussianTest(gaussian, img_gaussian);
    cout << "" << endl;
    cout << "Filtro GaussianBlur: " << endl;
    if (estado == 0) cout << "Exito total!" << endl;
    else cout << "Fallo en el proceso" << endl;


    Processing_APIS::bright_control(imgPtr,brightPtr,200);
    estado = testing.brightTest(bright,img_bright);
    cout << "" << endl;
    cout << "Filtro control de Brillo: " << endl;
    if (estado == 0) cout << "Exito total!" << endl;
    else cout << "Fallo en el proceso" << endl;


    Processing_APIS::gamma_correction(imgPtr,gammaPtr,2);
    estado = testing.gammaTest(gamma,img_gamma);
    cout<<""<<endl;
    cout<<"Filtro correcion Gamma: "<<endl;
    if (estado == 0) cout << "Exito total!" << endl;
    else cout<<"Fallo en el proceso"<<endl;

    Processing_APIS::gray_scale(imgPtr, grayPtr);
    estado = testing.grayTest(gray, img_gray);
    cout << "" << endl;
    cout << "Filtro escala de grises: " << endl;
    if (estado == 0) cout << "Exito total!" << endl;
    else cout << "Fallo en el proceso" << endl;
}





