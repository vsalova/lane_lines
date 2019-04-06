# Problem Discription

We will be working on the laneline detection problem. Laneline detection is a visial recognition problem where the task is to recognize where each of the laneline are in a scene.
Our plan is to work off of the pipeline from lab 1 from last quarter and to develop an alternative solution for parts where necessary to increase accuracy and speed.

# Motivation

Laneline detection is one of the most fundamental parts to autonomous driving. Following lane lines is the most basic part of a driving solution, ontop of which path planning and other parts is built. Knowing this, the laneline detection module must be robust, reliable, and accurate.

Even for non-self driving cars, laneline detection is the main component behind lane departure warning systems, and automatic steering in a highway enviornment.

# Previous Work

Spatial CNN - [link](https://arxiv.org/abs/1712.06080)

ICNet - [link](https://arxiv.org/abs/1704.08545)
Semantic segmentation, not lane-line detection
Downsamples a large input image by 1/2 and 1/4 and performs semantic segmentation on the smallest images first, then uses a "cascade feature fusion unit" to enhance the segmentation of the downsamples image to refine the segmentation in the higher resolution image.

LaneNet - [link](https://arxiv.org/abs/1807.01726)

DeepLabV3+ - [link](https://arxiv.org/abs/1802.02611)

Lab1 pipeline


# Timeline / Quarter Goals


#### Before Presentation

* Finding out the accuracy / speed of the lab 1 
* Clustering the Hough space implementation
  * Figure out best clustering algorithm (DBScan HDBScan?)
* Benchmark best models to the best models

##### After Presentations

* Record own video and test results on it
* Alternative for thresholding?
* Testing robustness for nighttime driving, poor conditions - better results?
