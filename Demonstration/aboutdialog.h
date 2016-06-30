/*******************************************************************************
* ECE 4805:        Senior Design Capstone
* Written by:      Kyle Imhof, SnapVision
* Last modified:   3 May 2016
* File name:       aboutdialog.h
* Description:     Header file for the About Dialog box
* Comments:
* Copyright (c) 2016, SnapVision
* All rights reserved.
*******************************************************************************/
#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui { class AboutDialog; }

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
