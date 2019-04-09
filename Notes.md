# Notes
Keep all notes here and project ideas here

## Calvin March 25th

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

## Calvin April 5th

Time for clustering -- bottleneck
K-means, sperical K-means | choose starting points smartly.
* Gloss
* CuGloss
* Thrust

------------------------------------------------------

## Calvin April 6th

Datasets
* CULane Dataset - from Beijing - Going to use- annotated - https://xingangpan.github.io/projects/CULane.html
* Caltech lane line dataset - US - not annotated - http://www.mohamedaly.info/datasets/caltech-lanes
* Use own camera - Calvin has dash cam - not annotated
* TuSimple - annotated - https://github.com/TuSimple/tusimple-benchmark/issues/3
* Cityscapes (semantic segmentation)
* CamVid (semantic segmentation)
* COCO-stuff (semantic segmentation)

## Daniel April 6th
Pipeline pieces alongside proposed modules:
* Perspective Transform, Use neural network 
* Color/gradient thresholds
* Window Search -Clustering, Spatial CNN, Kalman Filter
Lanenet proposes an entire pipeline, and such its implementation would require a new pipeline to be developed form the ground up.

Lanenet attempts to do end-to-end lane line detection with two neural networks. The first neural network handles binary and instance segmentation to produce a collection of pixels associated with a lane. These pixels are passed to the Hnet, which fits a curve to the lane line pixels. This approach looks promising but would require a ground up restructuring of our pipeline. Perhaps this should be attempted after we develop something to show xmotors?

------------------------------------------------------

## Everyone April 8th

TA Meeting

Since it's traditional computer vision, don't need labeled lanelines. Should work with any video.
Graph of clustering algorithms and time
##### NEED MORE WORK ON THREASHOLDING PART
Benchmark on embedded system
##### MAKE ON DOCKER
What labraries using for clustering
FPS is what for lab1?
OpenCV for GPU
