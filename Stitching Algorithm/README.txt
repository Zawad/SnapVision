SnapVision
This is an application built to stitch stock video. 

To compile these source files use compile_opencv.sh
Usage Example:
	./compile_opencv.sh snapvision.cpp	

snapvision.cpp   		- Is the first iteration of our project
				   		  Usage: ./snapvision <video file>
				   		  Results in one stitched image (result.jpg) for the whole video 

stitching.sh 			- This script combines the execution of x_snapvision.cpp and 	
							stitching_detailed.cpp to demonstrate the second iteration of our project
						  Usage: ./stitching.sh <video file>

part of stitching.sh:


x_snapvision.cpp 		- Is the second iteration of our project
				   		  Usage: ./x_snapvision <video file>
				   		  Results in 4 stitched images (q1-q4)
stitching_detailed.cpp          - Is used to stitch the results of x_snapvision.cpp
				   		  Usage: ./x_snapvision q1.jpg q2.jpg q3.jpg q4.jpg
				   		  Results in one stitched image (x_result.jpg)

Notes:
- snapvision.cpp and x_snapvision.cpp both use the stitcher object provided by opencv, thus
  require the images to be stored in a vector before being stitched.
- stitching_detailed.cpp is able to stitch images of different sizes together thus has
  the potential to work in real time. Which is what we were working towards in our second iteration. 