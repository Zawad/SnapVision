/*******************************************************************************
* ECE 4805:        Senior Design Capstone
* Written by:      Kyle Imhof, SnapVision
* Last modified:   3 May 2016
* File name:       main.cpp
* Description:     Main entrance point for GUI application
* Comments:

* Copyright (c) 2016, SnapVision
* All rights reserved.
*******************************************************************************/
#include "mainwindow.h"
#include <QApplication>
#include <QVideoWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
