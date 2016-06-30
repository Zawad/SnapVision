/*******************************************************************************
* ECE 4805:        Senior Design Capstone
* Written by:      Kyle Imhof, SnapVision
* Last modified:   3 May 2016
* File name:       aboutdialog.cpp
* Description:     Source file for the About Dialog box
* Comments:

* Copyright (c) 2016, SnapVision
* All rights reserved.
*******************************************************************************/
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
