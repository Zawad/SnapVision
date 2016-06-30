FrontEnd GUI

Description:   An interface for viewing stitching process as a video is played. 
               A user can select from prerecorded video or open a file of their own.
	       In order to run this project you must have QT version 4 and up installed on your machine.
Created by:    SnapVision
Modified Last: 3 May 2016

***********************
        CLAs
***********************
The program takes no command line arguments, it is run with './FrontEnd'


***********************
        Usage
***********************
To compile and run the GUI application, do the following in the front end directory:
	qmake
	make
	./FrontEnd

In order to add new videos to the demonstration, complete the following steps:
1) Run add_video.sh to add the images needed to the correct directory
	Usage: ./add_video.sh <video file in mp4 format> <desired directory name>
2) In the GUI use "Open" menu item to select the new video file from the desired directory name
3) Press start button to view video and stitched image

The program features a standard set of Menu Bar items, including a Full-Screen toggle,
an 'Exit' button, and an 'About' button. In the 'Stitching' tab, the user can select 
from preloaded videos using the combo box in the top-right, or use the 'Open' menu item
to select their own video. Once a video is selected, the video playback buttons in the
bottome left of the GUI can be used to control the video.

***********************
        Notes
***********************
- The user must change the 'localPath' variable in 'mainwindow.cpp' at line 32
- The stitched images produced in the 'Stitched Image' come from a predefined set of images that are placed in a specific subdirectory in the project directory. In order to load your own video file, you must ensure that the folder hierarchy follows the same pattern.