////////////////////////////////////////////////////////////////////////////////////////
//   ECE 4806 - Senior Design 
//         SnapVision
//   File: demo.cpp
//   Description: This file contains the program that loops through a stock video input 
//                and produces 4 stitched results of the videos. It prepares the necessary
//                files to run the front end
//   Copyright (c) 2016, SnapVision
//   All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <stdio.h>

#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching/stitcher.hpp"


using namespace cv;
using namespace std;

#define get_ticks cv::getTickCount
#define get_freq  cv::getTickFrequency
double dt(int64 t) { return double(t*1000/get_freq())/1000.0; }

/*
* C++ Profiler can be used to track runtime statistics on an application and even methods in side an applicaiton. 
* Referenced code written by berak from Stackoverflow
*/
struct Profile 
{
    string name;
    int64 t; // accumulated time
    int64 c; // function calls

    Profile(const string & name) 
        : name(name)
        , t(0) 
        , c(0)
    {}   

    ~Profile() 
    {
        cerr << format("%-19s %9u ",name.c_str(),c);
        cerr << format("%14.6f ",dt(t/c)); 
        cerr << format("%14.6f ",dt(t));
        cerr << endl;
    }

    struct Scope
    {
        Profile & p;
        int64 t;

        Scope(Profile & p) 
            : p(p) 
            , t(get_ticks()) 
        {}

        ~Scope() 
        { 
            int64 t1 = get_ticks();
            if ( t1 > t )
            {
                p.t += t1 - t;
                p.c ++;
            }
         }
    }; 
};

#define PROFILEX(s) static Profile _a_rose(s); Profile::Scope _is_a_rose_is(_a_rose);
#define PROFILE PROFILEX(__FUNCTION__)

bool try_use_gpu = true;
vector<Mat> imgs;
string result_name = "result.jpg";
string window_name = "SnapVision";

//hide the local functions in an anon namespace
namespace {
    //help method used to explain the program to the user
    void help(char** av) {
        cout << "The program captures frames from a video file and produces 4 stitched results." << endl;
        cout << "Usage:\n" << av[0] << " <video file>" << endl;
    }
    /*
    * stitch is a method that attempts to stitch a vector of images and returns whether or not it is succesfull.  
    * param pano - holds the image containing the results of the stitching
    * returns - whether or not the stitch was completed
    */
    bool stitch(Mat pano){
        PROFILE;
        Stitcher stitcher = Stitcher::createDefault(try_use_gpu);
        Stitcher::Status status = stitcher.stitch(imgs, pano);
        
        if (status != Stitcher::OK)
        {        
            cout << "Can't stitch images, error code = " << int(status) << endl;
            return false;
        }else{
            imwrite(result_name, pano);
            return true;
        }
    }
    /*
    * processes is a method that loops through a whole video and then calls on the stitcher mehtod 
    *           to stitch the images together
    * param capture - holds the video capture of the input stock video 
    * returns - whether or not the stitch was completed
    */
    int process(VideoCapture& capture) {
        PROFILE;
        int n = 0;
        char filename[200];
        
        Mat init_frame;
        int i = 0;
        int count = capture.get(CV_CAP_PROP_FRAME_COUNT);
        double fps = capture.get(CV_CAP_PROP_FPS);
        int skip_frames = 35;
        cout << "Frame Count: " << count << endl;
        cout << "Frames Per Second: " << fps << endl;
        cout << "Number of Stiches: " << count/skip_frames << endl;
        capture >> init_frame;
        imgs.push_back(init_frame);
        //while the video frame is not empty keep capturing the frames 
        //and storing the frames into a vector of images
        while (1)
        {
            Mat frame;
            capture >> frame;
            if (frame.empty())
                break;
            if(i%skip_frames == 0){
                imgs.push_back(frame);
            }
            frame.release();
            i++;
        }
        vector<Mat> demo;
        demo.push_back(imgs[1]);
        //loop through the images and create a stich for each incremental frame captured
        for (int i = 2; i < imgs.size(); i++)
        {

            demo.push_back(imgs[i]);
            cout << demo.size() << endl;
            Mat pano;
            Stitcher stitcher = Stitcher::createDefault(try_use_gpu);
            Stitcher::Status status = stitcher.stitch(demo, pano);
            if (status != Stitcher::OK)
            {        
                cout << "Can't stitch images, error code = " << int(status) << endl;
                return false;
            }else{
                sprintf(filename,"demo%.2d.jpg",n++);
                imwrite(filename, pano);
            }
        }
        Mat pano;
        pano.release();
        imgs.clear();
        return 0;
    }
    /*
    * showMosaic is a method that shows the mosaic
    */
    void showMosaic(){
        PROFILE;
        namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
        imshow(window_name, imread(result_name));
        while(1){
            char key = (char)waitKey(30); //delay N millis, usually long enough to display and capture input
            if(key == 27) //escape key
            {
                cout<< "Program Runtime:\t Num Calls  Time/Call\t     Time"<<endl;
                return; 
            }
        }
    }
}

int main(int ac, char** av) {
    PROFILE;
    if (ac != 2) {
        help(av);
        return 1;
    }
    std::string arg = av[1];
    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file or image sequence
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened()) {
        cerr << "Failed to open the video device, video file or image sequence!\n" << endl;
        help(av);
        return 1;
    }
    if(process(capture)){
        showMosaic();
    }
    return 0;
}
