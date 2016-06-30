#!/bin/bash
#this file is a script that allows the user create the folder
#structure necessary to add a new video to the FrontEnd GUI 
#using SnapVision's stitching algorithm (demo.cpp)
#After the folder structure is created the 
#the directory containing the new video is moved
#to the directory above and now is ready to be displayed
#via the FrontEnd GUI
#Copyright (c) 2016, SnapVision
#All rights reserved.

if [[ $1 == *.mp4 ]]
then
	./compile_opencv.sh demo.cpp
	echo "stitching initiated"
	./demo $1
	mkdir $2
	cd $2 
	mkdir imgs
	cd ..
	mv demo*.jpg ./$2/imgs
	mv $1 ./$2
	mv $2 ../
else
	echo "Usage ./add_video.sh <video file in mp4> <directory name>"
	echo "Example ./add_video.sh trees_aerial.mp4 trees"

fi

