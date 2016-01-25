// Librerias de Qt
#include "mainwindow.h"
#include <QApplication>

// Librerias de C++
#include <iostream>

// Include relacionados a opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>

// Para ahorrar tiempo y agrgar claridad, aunque un tanto abstracto,
// agregamos los espacios de nombes d eC++ y OpenCV


using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Mat imagen;

    imagen = imread("saitama.jpg", CV_LOAD_IMAGE_COLOR);

    namedWindow("Saitama", 1);

    if(!imagen.empty())
    {
        imshow("Saitama", imagen);
        waitKey(1000);
    }
    else
    {
        w.show();
    }

    VideoCapture cap(0);



    return a.exec();
}
