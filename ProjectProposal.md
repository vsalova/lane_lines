# Problem Discription

We will be working on the lane line detection problem. Lane line detection is a visual recognition problem where the task is to recognize where each of the lane line are in a scene. There are many various techniques for lane line detection, but the main issues are the speed and accuracy tradeoff: many algorithms that provide accurate results are often too slow to run in real-time. Some of the accuracy could be sacrificed in order to allow real-time lane line detection, but it is still a problem to have have accurate enough results that are computed fast enough for real time detection. 


Our plan is to work off of the pipeline from lab 1 from last quarter and to develop an alternative solution for parts where necessary to increase accuracy and speed, while keeping the same general structure of the code. 

# Motivation

Lane line detection is one of the most fundamental parts to autonomous driving. Following lane lines is the most basic part of a driving solution, on top of which path planning and other parts is built. Knowing this, the lane line detection module must be robust, reliable, and accurate. Even for non-self driving cars, lane line detection is the main component behind lane departure warning systems, and automatic steering in a highway environment.

# Previous Work

### Spatial CNN - [link](https://arxiv.org/abs/1712.06080)

Spatial CNN is a generalization of deep convolutional to a spatial level. SCNN views rows and columns of feature maps as layers and applies convolution and nonlinear activation sequentially, forming a deep neural network. The advantages of SCNN over other lane line detection methods are the fact that sequential message pass scheme is more computationally efficient than other RNN or Markov Random Field approaches. Spatial CNN is computationally efficient and also flexible. SCNN distinguishes each lane line as its own class, creates a probability map for each, which give prediction on the existence of the lane line markings. After that, these predictions are connected by cubic splines to create the final predictions of the lane lines. In this paper, SCNN was compared to ReNet, MRFNet, DenseCRF, and the very deep residual network. Out of all the methods listed, SCNN performed significantly better, showing the best results.  

### ICNet - [link](https://arxiv.org/abs/1704.08545)  
Semantic segmentation, not lane-line detection
Downsamples a large input image by 1/2 and 1/4 and performs semantic segmentation on the smallest images first, then uses a "cascade feature fusion unit" to enhance the segmentation of the downsamples image to refine the segmentation in the higher resolution image.
ICNet achieves memory and speed improvements in the order of 5x to previous semantic segmentation models while maintaining good accuracy. (tables 4,5,6 in the paper)

### LaneNet - [link](https://arxiv.org/abs/1807.01726)
Proposes an Entire lane line detection pipeline. 

Lanenet attempts to do end-to-end lane line detection with two neural networks, training on a 1080 ti. The first neural network handles binary and instance segmentation to produce a collection of pixels associated with a lane. These pixels are passed to the Hnet, which fits a curve to the lane line pixels. This approach looks promising but would require a ground up restructuring of our pipeline. Perhaps this should be attempted after we develop something to show xmotors?

### DeepLabV1,2,3,3+
* [DeepLabV1](https://arxiv.org/pdf/1412.7062.pdf)
DeepLabV1 uses "atrous convolutions" as one part of its model, these convolutions include a "rate" which is the step size between each pixel in the kernel. A rate of 1 is a normal convolution, and a rate of 2 with a 3x3 kernel would be like a 5x5 kernel but with the 2nd and 4th rows and columns not included.
DeepLabV1 also uses Fully Connected Conditional Random Fields to refine the rough localization of semantic parts from the Deep CNN parts. Basically, the output of the DCNN looks like a heatmap, and Fully Connected Conditional Random Fields find the boundary of all the segments.
DeepLabV1 talks about using Multi-Scale Prediction, which accomplishes the same thing as Fully Connected Conditional Random Fields but not as well.

* [DeepLabV2](?)
Adds Atrous Spatial Pyrimid Pooling (ASPP) to help find objects of different size and for computational efficiency.

* [DeepLabV3](https://arxiv.org/pdf/1706.05587.pdf)
Better ASPP with image level features
Adds batch normalization parameters for training

* [DeepLabV3+](https://arxiv.org/abs/1802.02611)
Adds a decoder module that refines segmentation along object boundaries, which has the ability to choose between precision and run-time.

Overall, DeepLab is one of the best semantic segmentation models currently available.
DeepLabV1 and V2 both run at around 1 fps, which is way too slow.
DeepLabV3 training on the PASCAL VOC 2012 semantic segmentation dataset with a Tesla K80 GPU took 3.65 days to train.

[Here](http://web.eng.tau.ac.il/deep_learn/wp-content/uploads/2017/12/Rethinking-Atrous-Convolution-for-Semantic-Image-Segmentation-1.pdf) is a presentation of the overview of each of the models.

---------------------------------------------------------------------

### Lab 1 pipeline

Pipeline pieces alongside proposed modules:

* Perspective Transform, Use neural network 
* Color/gradient thresholds
* Window Search -Clustering, Spatial CNN, Kalman Filter

Lanenet proposes an entire pipeline, and such its implementation would require a new pipeline to be developed form the ground up.

# Timeline / Quarters Goals

#### Before Presentation

* Refactor/combine the code from lab 1
* Finding out the accuracy/speed of the lab 1 
* Replace window search with clustering the Hough space implementation
  * Figure out best clustering algorithm (DBScan, HDBScan)
  * Create a basic structure and improve upon it
* Benchmark best models to the best models (LaneNet, SCNN)

#### After Presentation

* Record own video and test results on it
* Train and test on different datasets (CULane, Cityscapes, etc)
* Alternative for thresholding
* Testing robustness for nighttime driving, poor conditions - better results
* Apply best segmentation models to lane-line detection, benchmark them and compare against laneline detection algorithms 
