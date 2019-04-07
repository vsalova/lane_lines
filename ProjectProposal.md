# Problem Discription

We will be working on the laneline detection problem. Laneline detection is a visial recognition problem where the task is to recognize where each of the laneline are in a scene. There are many various techniques for laneline detection, but the main issues are the speed and accuracy tradeoff: many algorithms that provide accurate results are often too slow to run in real-time. Some of the accuracy could be sacrificed in order to allow real-time laneline detection, but it is still a problem to have have accurate enough results that are computed fast enough for real time detection. 


Our plan is to work off of the pipeline from lab 1 from last quarter and to develop an alternative solution for parts where necessary to increase accuracy and speed.

# Motivation

Laneline detection is one of the most fundamental parts to autonomous driving. Following lane lines is the most basic part of a driving solution, on top of which path planning and other parts is built. Knowing this, the laneline detection module must be robust, reliable, and accurate.

Even for non-self driving cars, laneline detection is the main component behind lane departure warning systems, and automatic steering in a highway enviornment.

# Previous Work

Spatial CNN - [link](https://arxiv.org/abs/1712.06080)

ICNet - [link](https://arxiv.org/abs/1704.08545)
Semantic segmentation, not lane-line detection
Downsamples a large input image by 1/2 and 1/4 and performs semantic segmentation on the smallest images first, then uses a "cascade feature fusion unit" to enhance the segmentation of the downsamples image to refine the segmentation in the higher resolution image.
ICNet achieves memory and speed improvements in the order of 5x to previous segmantic segmentation models while maintaining good accuracy. (tables 4,5,6 in the paper)

LaneNet - [link](https://arxiv.org/abs/1807.01726)

DeepLabV3+ - [link](https://arxiv.org/abs/1802.02611)

Lab1 pipeline


# Timeline / Quarters Goals


#### Before Presentation

* Finding out the accuracy / speed of the lab 1 
* Clustering the Hough space implementation
  * Figure out best clustering algorithm (DBScan HDBScan?)
* Benchmark best models to the best models (LaneNet + others?)

##### After Presentations

* Record own video and test results on it
* Alternative for thresholding?
* Testing robustness for nighttime driving, poor conditions - better results?
