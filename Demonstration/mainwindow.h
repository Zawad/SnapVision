/*******************************************************************************
* ECE 4805:        Senior Design Capstone
* Written by:      Kyle Imhof, SnapVision
* Last modified:   3 May 2016
* File name:       mainwindow.h
* Description:     Header file for the main window of the GUI application
* Comments:
*******************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QGraphicsPixmapItem>
#include <QtWidgets>
#include "aboutdialog.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /* Widget slot connections */
    void on_exitButton_clicked();
    void on_startButton_clicked();
    void on_pauseButton_clicked();
    void on_playButton_clicked();
    void on_restartButton_clicked();
    void on_stopButton_clicked();
    void on_backButton_clicked();
    void on_selectComboBox_currentIndexChanged(int index);

    /* Timer Callbacks */
    void onDurationTimerTimeout();
    void onImgChangeTimerTimeout();

    /* Actions */
    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_actionAbout_SnapVision_triggered();
    void on_actionToggle_Full_Screen_triggered();

private:
    Ui::MainWindow *ui;
    AboutDialog about;

    void openFromComboBox(QString path);

    qint64 mVideoDuration;
    qint64 mInterval;
    qint64 mImgCount;

    QTimer *mDurationTimer;
    QTimer *mImgChangeTimer;

    QString localPath;
    QString mPath;
    QString mFileName;
    QStringList mFileList;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *image;
};

#endif // MAINWINDOW_H
