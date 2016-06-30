/*******************************************************************************
* ECE 4805:        Senior Design Capstone
* Written by:      Kyle Imhof, SnapVision
* Last modified:   3 May 2016
* File name:       mainwindow.cpp
* Description:     Main window for GUI application to demonstrate stitching process
*                  User selectable videos and resulting stitched images are displayed
* Comments:        The user of this program needs to change the 'localPath' variable

* Copyright (c) 2016, SnapVision
* All rights reserved.
*******************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* This timer waits for the video duration to be calculated, then calculates the interval *
     * on which to add in the next image stitch.                                              */
    mDurationTimer = new QTimer(this);
    connect(mDurationTimer, SIGNAL(timeout()), this, SLOT(onDurationTimerTimeout()));
    mDurationTimer->start(200);

    /* This timer waits for the video duration value mod the interval value to be equal to 0, *
     * at which time the next image is added to the result image.                             */
    mImgChangeTimer = new QTimer(this);
    connect(mImgChangeTimer, SIGNAL(timeout()), this, SLOT(onImgChangeTimerTimeout()));

    /* This is the primary widget in the scene that allows for video playback. */
    player = new QMediaPlayer(this);
    player->setVideoOutput(ui->videoWidget);

    localPath = "C:/Users/Kyle/Documents/Virginia Tech/Classes/ECE4805_SeniorDesignCapstone/Qt/final_demo/FrontEnd/";

    ui->selectComboBox->addItem("Select a video to stitch...");
    ui->selectComboBox->addItem("Trees Up A Mountain");
    ui->selectComboBox->addItem("Aerial View of London");
    ui->selectComboBox->addItem("Scene Over a Highway");
    ui->selectComboBox->addItem("Scene Over a Building");

    /* This sets the resulting images in the "Results" tab. */
    ui->label_2->setPixmap(QPixmap(localPath+"building/imgs/demo19.jpg"));
    ui->label_3->setPixmap(QPixmap(localPath+"london/imgs/demo12.jpg"));
    ui->label_4->setPixmap(QPixmap(localPath+"trees/imgs/demo10.jpg"));
    ui->label_5->setPixmap(QPixmap(localPath+"highway/imgs/demo29.jpg"));

    this->showFullScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

/* If the Open function is used, it is necessary for the selected file to be contained in a folder *
 * hierarchy like the default videos, and must have 'imgs' as the folder name for the images.      */
void MainWindow::on_actionOpen_triggered()
{
    mPath = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), "All Files (*.*)");
    QFileInfo fileInfo(mPath);
    mFileName = fileInfo.fileName();
    mPath = mPath.remove(mFileName);
    player->setMedia(QMediaContent(QUrl::fromLocalFile(mPath+mFileName)));
    mDurationTimer->start(200);
    qDebug() << mPath;
    qDebug() << mFileName;

    QDir vidDir(mPath+"imgs");
    mFileList = vidDir.entryList();
    mFileList.removeFirst();
    mFileList.removeFirst();
    mImgCount = 0;
    qDebug() << mFileList;

}

void MainWindow::on_exitButton_clicked()
{
    qApp->exit();
}

void MainWindow::on_startButton_clicked()
{
    player->play();
    mImgChangeTimer->start(1);
    if (player->position() == mVideoDuration) {
        mImgCount = 0;
    }
}

void MainWindow::on_pauseButton_clicked()
{
    player->pause();
}

void MainWindow::on_playButton_clicked()
{
    player->play();
}

void MainWindow::on_restartButton_clicked()
{
    on_startButton_clicked();
}

void MainWindow::on_stopButton_clicked()
{
    player->setMedia(QMediaContent(QUrl::fromLocalFile(mPath)));
    if (player->position() == mVideoDuration) {
        mImgCount = 0;
    }
    ui->stitchImgLabel->setPixmap(QPixmap(QString::fromUtf8(":/Icons/logo.png")));
}

void MainWindow::on_actionAbout_SnapVision_triggered()
{
    about.exec();
}

void MainWindow::onDurationTimerTimeout()
{
    if( player->duration() != -1 )
    {
        mVideoDuration = player->duration();
        mInterval = mVideoDuration/mFileList.size();
        mDurationTimer->stop();
    }
}

void MainWindow::onImgChangeTimerTimeout()
{
    if( (player->position() != 0) && (player->position()%mInterval == 0) && (mVideoDuration != -1) && (mImgCount < mFileList.size()))
    {
        qDebug() << "Count: " << player->position() << " "<< mImgCount << " " << mFileList.size() << " " << mInterval << " " << mVideoDuration;
        QString p = mPath+"imgs/"+mFileList.at(mImgCount);
        qDebug() << p;
        ui->stitchImgLabel->setPixmap(QPixmap(p));
        mImgCount++;
    }
    if( mImgCount == mFileList.size() )
    {
        QString p = mPath+"imgs/"+mFileList.at(mImgCount-1);
        qDebug() << "ct: " << mImgCount-1;
        ui->finalStitchLabel->setPixmap(QPixmap(p));
        ui->stackedWidget->setCurrentIndex(1);
        mImgCount = 0;
        player->setPosition(0);
    }
}

void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->stitchImgLabel->setPixmap(QPixmap(QString::fromUtf8(":/Icons/logo.png")));
}

void MainWindow::openFromComboBox(QString path)
{
    QFileInfo fileInfo(path);
    mFileName = fileInfo.fileName();
    mPath = path.remove(mFileName);
    player->setMedia(QMediaContent(QUrl::fromLocalFile(mPath+mFileName)));
    mDurationTimer->start(200);
    qDebug() << mPath;
    qDebug() << mFileName;

    QDir vidDir(mPath+"imgs");
    mFileList = vidDir.entryList();
    mFileList.removeFirst();
    mFileList.removeFirst();
    mImgCount = 0;
    qDebug() << mFileList;

}

void MainWindow::on_selectComboBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            ui->stitchImgLabel->setPixmap(QPixmap(QString::fromUtf8(":/Icons/logo.png")));
            mImgCount = 0;
            player->setPosition(0);
            player->pause();
        break;
        case 1:
            openFromComboBox(QString(localPath + "trees/trees_aerial.mp4"));
            break;
        case 2:
            openFromComboBox(QString(localPath + "london/london_aerial.mp4"));
            break;
        case 3:
            openFromComboBox(QString(localPath + "highway/highway_aerial.mp4"));
            break;
        case 4:
            openFromComboBox(QString(localPath + "building/building_aerial.mp4"));
            break;
        default:
            qDebug() << "Shouldn't be here in MainWindow::on_selectComboBox_currentIndexChanged";
        break;
    }
}

void MainWindow::on_actionToggle_Full_Screen_triggered()
{
    if(!this->isFullScreen())
    {
        this->showFullScreen();
    }
    else
    {
        this->showNormal();
    }
}
