////////////////////////////////////////////////////////////////////////////////////////
//   ECE 4806 - Senior Design 
//         SnapVision
//   File: x_snapvision.cpp
//   Description: This file contains the program that loops through a stock video input 
//                and produces 4 stitched results of the videos. 
//
//   Copyright (c) 2016, SnapVision
//   All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>


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
        //cerr << format("%14u",t);
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
#define PROFILE PROFILEX(__FUNCTION__);

static int NUM_THREADS = 4;
bool try_use_gpu = true;
vector<Mat> imgs;
string result_name = "result.jpg";
string window_name = "SnapVision";
Mat q1_mosaic;
Mat q2_mosaic;
Mat q3_mosaic;
Mat q4_mosaic;

//hide the local functions in an anon namespace
namespace {
    //help method used to explain the program to the user
    void help(char** av) {
        cout << "The program captures frames from a video file and produces 4 stitched results." << endl
             << "Usage:\n" << av[0] << " <video file>" << endl;
    }
    /*
    * stitch is a method that attempts to stitch a vector of images and returns whether or not it is succesfull.  
    * param pano - holds the image containing the results of the stitching
    * returns - whether or not the stitch was completed
    */
    bool stitch(vector<Mat> imgs, Mat pano){
        PROFILE;
        Stitcher stitcher = Stitcher::createDefault(try_use_gpu);
        Stitcher::Status status = stitcher.stitch(imgs, pano);
        static int n = 0;
        char filename[100];

        if (status != Stitcher::OK)
        {        
            cout << "Can't stitch images, error code = " << int(status) << endl;
            return false;
        }else{
            sprintf(filename, "result%.3d.jpg",n++);
            imwrite(filename, pano);
            return true;
        }
    }
    /*
    * thread_data struct
    * id an integer that serves as an id
    * the vector of images holds the images that will be stitched in the separate threads
    */
    struct thread_data{
        int  id;
        vector<Mat> imgs;
    };
    /*
    * stitch_async is a method that handles the stitching based on the thread id
    */
    void* stitch_async(void* data)
    {
        PROFILE;
        Mat pano;
        struct thread_data *t_data;
        t_data = (struct thread_data *) data;
        char filename[100];
        
        Stitcher stitcher = Stitcher::createDefault(try_use_gpu);
        Stitcher::Status status = stitcher.stitch(t_data->imgs, pano);
        if (status != Stitcher::OK)
        {        
            cout << "Can't stitch images, error code = " << int(status) << endl;
            cout << "Thread Id = " << int(t_data->id) << endl;
        }else{
            sprintf(filename, "q%.1d.jpg",t_data->id);
            //imwrite(filename, pano);
            switch(t_data->id){
                case 1:
                    q1_mosaic = pano;
                    imwrite(filename,q1_mosaic);
                    break;
                case 2:
                    q2_mosaic = pano;
                    imwrite(filename,q2_mosaic);
                    break;
                case 3:
                    q3_mosaic = pano;
                    imwrite(filename,q3_mosaic);
                    break;
                case 4:
                    q4_mosaic = pano;
                    imwrite(filename,q4_mosaic);
                    break;
                default:
                    break;
            }
        }
        //pthread_exit(NULL);
        return NULL;
    }
    /*
    * kill frees up used memory.
    */
    void kill() {
        PROFILE;
        for(int i = 0; i < imgs.size(); i++){
            imgs[i].release();
        }
        q1_mosaic.release();
        q2_mosaic.release();
        q3_mosaic.release();
        q4_mosaic.release();
    }
    /*
    * splits up the total capture frames into 4
    */
    void vectorSplit(){
        PROFILE;
        Mat pano;
        vector<Mat> q1,q2,q3,q4;
        for (int j = 0; j < imgs.size(); j++)
        {
            /* code */
            if(j<imgs.size()/4){
                q1.push_back(imgs[j]);
            }else if(j< imgs.size()/2){
                q2.push_back(imgs[j]);
            }else if(j< 3*imgs.size()/4){
                q3.push_back(imgs[j]);
            }else if(j<imgs.size()){
                q4.push_back(imgs[j]);
            }
        }
        
        struct thread_data td[NUM_THREADS];
        pthread_t threads[NUM_THREADS];
        pthread_attr_t attr;
        void* status;

        // Initialize and set thread joinable
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        int rc;
        int i;
        for (i = 0; i < NUM_THREADS; i++)
        {
            td[i].id = i+1;
            switch(td[i].id){
                case 1:
                    td[i].imgs = q1;
                    break;
                case 2:
                    td[i].imgs = q2;
                    break;
                case 3:
                    td[i].imgs = q3;
                    break;
                case 4:
                    td[i].imgs = q4;
                    break;
                default:
                    break;
            }
            rc = pthread_create(&threads[i], NULL,
                              stitch_async, (void *)&td[i]);
            if (rc){
                cout << "Error:unable to create thread," << rc << endl;
                exit(-1);
            }
        }

        // free attribute and wait for the other threads
        pthread_attr_destroy(&attr);
        for( i=0; i < NUM_THREADS; i++ ){
           rc = pthread_join(threads[i], &status);
           if (rc){
              cout << "Error:unable to join," << rc << endl;
              exit(-1);
           }
        }
        for(i = 0; i < NUM_THREADS; i++){
            q1[i].release();
            q2[i].release();
            q3[i].release();
            q4[i].release();
        }
        imgs.clear();
        kill();
        return;
    }
    /*
    * processes is a method that loops through a whole video and then calls on the vectorSplit mehtod 
    *           to produce the stitched results
    * param capture - holds the video capture of the input stock video 
    * returns - whether or not the stitch was completed
    */
    int process(VideoCapture& capture) {
        PROFILE;
        int n = 0;
                
        Mat init_frame;
        int i = 0;
        int count = capture.get(CV_CAP_PROP_FRAME_COUNT);
        //capture.set(CV_CAP_PROP_FPS, 10);
        double fps = capture.get(CV_CAP_PROP_FPS);
        int skip_frames = 35;//75;//25;//50;
        cout << "Frame Count: " << count << endl;
        cout << "Frames Per Second: " << fps << endl;
        cout << "Number of Stiches: " << count/skip_frames << endl;
        capture >> init_frame;
        imgs.push_back(init_frame);      
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
        vectorSplit();
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
