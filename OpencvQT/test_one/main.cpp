// Librerias de Qt
#include "mainwindow.h"
#include <QApplication>

// Librerias de C++
#include <iostream>

// Include relacionados a opencv
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"

// Para ahorrar tiempo y agrgar claridad, aunque un tanto abstracto,
// agregamos los espacios de nombes d eC++ y OpenCV


using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Mat imagen;

    imagen = imread("saitama.jpg", IMREAD_COLOR);

    namedWindow("Quetzal", WINDOW_AUTOSIZE);

    imshow("Quetzal", imagen);
    waitKey(0);
    cin.get();
    return a.exec();
}
