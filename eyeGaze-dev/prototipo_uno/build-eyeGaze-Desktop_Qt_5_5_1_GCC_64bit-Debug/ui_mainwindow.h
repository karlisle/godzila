/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionSettings;
    QAction *actionCredits;
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QCommandLinkButton *recordCameraLinkButton;
    QCommandLinkButton *previewCameraLinkButton;
    QCommandLinkButton *trainCameraLinkButton;
    QCommandLinkButton *detectCameraLinkButton;
    QCommandLinkButton *gazeCameraLinkButton;
    QGroupBox *groupBox_2;
    QCommandLinkButton *previewFileLinkButton;
    QCommandLinkButton *detectFileLinkButton;
    QCommandLinkButton *recordFileLinkButton;
    QCommandLinkButton *trainFileLinkButton;
    QCommandLinkButton *gazeFileLinkButton;
    QLabel *label;
    QFrame *line;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(569, 480);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QStringLiteral("actionSettings"));
        actionCredits = new QAction(MainWindow);
        actionCredits->setObjectName(QStringLiteral("actionCredits"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(5, 2, 250, 400));
        recordCameraLinkButton = new QCommandLinkButton(groupBox);
        recordCameraLinkButton->setObjectName(QStringLiteral("recordCameraLinkButton"));
        recordCameraLinkButton->setGeometry(QRect(10, 116, 200, 40));
        previewCameraLinkButton = new QCommandLinkButton(groupBox);
        previewCameraLinkButton->setObjectName(QStringLiteral("previewCameraLinkButton"));
        previewCameraLinkButton->setGeometry(QRect(10, 32, 200, 40));
        trainCameraLinkButton = new QCommandLinkButton(groupBox);
        trainCameraLinkButton->setObjectName(QStringLiteral("trainCameraLinkButton"));
        trainCameraLinkButton->setGeometry(QRect(10, 158, 221, 40));
        detectCameraLinkButton = new QCommandLinkButton(groupBox);
        detectCameraLinkButton->setObjectName(QStringLiteral("detectCameraLinkButton"));
        detectCameraLinkButton->setGeometry(QRect(10, 74, 200, 40));
        gazeCameraLinkButton = new QCommandLinkButton(groupBox);
        gazeCameraLinkButton->setObjectName(QStringLiteral("gazeCameraLinkButton"));
        gazeCameraLinkButton->setGeometry(QRect(10, 200, 200, 40));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(290, 2, 250, 400));
        previewFileLinkButton = new QCommandLinkButton(groupBox_2);
        previewFileLinkButton->setObjectName(QStringLiteral("previewFileLinkButton"));
        previewFileLinkButton->setGeometry(QRect(1, 31, 200, 40));
        detectFileLinkButton = new QCommandLinkButton(groupBox_2);
        detectFileLinkButton->setObjectName(QStringLiteral("detectFileLinkButton"));
        detectFileLinkButton->setGeometry(QRect(1, 73, 200, 40));
        recordFileLinkButton = new QCommandLinkButton(groupBox_2);
        recordFileLinkButton->setObjectName(QStringLiteral("recordFileLinkButton"));
        recordFileLinkButton->setGeometry(QRect(1, 115, 200, 40));
        trainFileLinkButton = new QCommandLinkButton(groupBox_2);
        trainFileLinkButton->setObjectName(QStringLiteral("trainFileLinkButton"));
        trainFileLinkButton->setGeometry(QRect(1, 157, 200, 40));
        gazeFileLinkButton = new QCommandLinkButton(groupBox_2);
        gazeFileLinkButton->setObjectName(QStringLiteral("gazeFileLinkButton"));
        gazeFileLinkButton->setGeometry(QRect(1, 199, 200, 40));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 240, 240, 161));
        label->setAutoFillBackground(false);
        label->setStyleSheet(QLatin1String("background-image: url(:/cv_lab.jpg);\n"
" background-repeat: no-repeat;"));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(265, 2, 3, 400));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 569, 28));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionSettings);
        menuHelp->addAction(actionCredits);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Eye Gaze Estimation Demo", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", 0));
        actionCredits->setText(QApplication::translate("MainWindow", "Credits", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Modo Camara", 0));
        recordCameraLinkButton->setText(QApplication::translate("MainWindow", "Registrar iris ", 0));
        previewCameraLinkButton->setText(QApplication::translate("MainWindow", "Previsualizar ", 0));
        trainCameraLinkButton->setText(QApplication::translate("MainWindow", "Entrenar estimador de mirada", 0));
        detectCameraLinkButton->setText(QApplication::translate("MainWindow", "Detectar iris ", 0));
        gazeCameraLinkButton->setText(QApplication::translate("MainWindow", "Predecir mirada", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Desde archivo", 0));
        previewFileLinkButton->setText(QApplication::translate("MainWindow", "Previsualizar", 0));
        detectFileLinkButton->setText(QApplication::translate("MainWindow", "Detectar iris", 0));
        recordFileLinkButton->setText(QApplication::translate("MainWindow", "Guardar registro de iris", 0));
        trainFileLinkButton->setText(QApplication::translate("MainWindow", "Entrenar estimador de mirada", 0));
        gazeFileLinkButton->setText(QApplication::translate("MainWindow", "Estimador de mirada", 0));
        label->setText(QString());
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
