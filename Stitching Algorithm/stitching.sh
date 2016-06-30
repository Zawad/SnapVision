#!/bin/bash
#this file is a script that allows the user to emulate dividing the
#process into four threads
#Creates four stitched images via the x_snapvision cpp file
#Uses these four stitched images to create the final stitched image
#using the stitching_detailed.cpp
#Copyright (c) 2016, SnapVision
#All rights reserved.



if [[ $1 == *.mp4 ]]
then
	echo "stitching initiated"
	./compile_opencv.sh x_snapvision.cpp
	./x_snapvision $1
	if [ -f q1.jpg ] && [ -f q2.jpg ] && [ -f q3.jpg ] && [ -f q4.jpg ]
	then
		./compile_opencv.sh stitching_detailed.cpp
		./stitching_detailed q1.jpg q2.jpg q3.jpg q4.jpg
		#open the result image
		xdg-open x_result.jpg
	fi
else
	echo "Usage ./stitching.sh <video file in mp4>"
	echo "Example ./stitching.sh building_aerial.mp4"
fi

