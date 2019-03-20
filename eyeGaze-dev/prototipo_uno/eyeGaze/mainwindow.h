#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt cabeceras
#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QDir>

// Librerias estandar de  C++
#include <fstream>

// Cabeceras OpenCV

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

// Cabeceras de intraface

//#include <intraface/FaceAlignment.h>
//#include <intraface/XXDescriptor.h>




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
