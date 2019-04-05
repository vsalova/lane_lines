# Notes
Keep all notes here and project ideas here

### Calvin March 25th

------------------------------------------------------

##### Spatial CNN
* See paper
* See tensor flow implementation 	
	
------------------------------------------------------

##### Clustering

* Replacement for Windows search
* Haugh space
* Haugh transform takes a line (y = mx+b) and puts it into Haugh space of m and b

DB Scan
HDB Scan

Look at all clustering algorithms to see which ones work for lines

Input would be the binary warped image

------------------------------------------------------

##### Kalmann Filter 

Taking into account time -- Kalman filtering
	Requires that detection is already good
How else to take time into account?

Or also shift picture down by certain number of pixels that accounts for speed and 

------------------------------------------------------

##### Deep Lab neural net

* Best for semantic segmentation 
* Can use for lane line detection
* Needs 4 GPUs to train 
* Would be better than Spatial CNN if have enough batch size, batch size == number of GPUs

------------------------------------------------------

* Have bitmap of lane line pixels how to tell direction
* 10x10 bitmap get vector of direction???
* Easy way to get direction, point, which is fast?
* Start from center point to average of pixels on top row
* Use this step after the initial DBScan clustering, the convert the little vectors to the Haugh Space

------------------------------------------------------

##### Choosing only 2 lane lines:
* Split down the middle to choose one lane from each side
* Try to detect lines closest to 3.7m apart from histogram
* For histogram, choose certain % height of image from below to work with, less % for highly curved lane

------------------------------------------------------

How to tell how many lanes there are and where they start at the bottom of the image?

------------------------------------------------------

Post processing is just best fit line: input is just bitmap of the lane, all of the pixels of the lane

------------------------------------------------------

* Other domain to make things easier other than Haugh Space
* Need to look for more domains to represent vectors (point and slope)

Silouette clustering ?

DBScan

------------------------------------------------------

How to compare lane lines
Correct versus preducted 

------------------------------------------------------

Time for clustering -- bottleneck
K-means, sperical K-means | choose starting points smartly.
* Gloss
* CuGloss
* Thrust

------------------------------------------------------

