import hdbscan
import debacl
import fastcluster
import sklearn.cluster
import scipy.cluster
import sklearn.datasets
import numpy as np
import pandas as pd
import time
import matplotlib.pyplot as plt
import seaborn as sns

#def removeOutliers(dataset, outliers):


filepath = "hough_example_output_noise1.txt"

f = open(filepath)
dataset = np.loadtxt(f, delimiter=',', usecols=(0,1), unpack=False)

f = open(filepath)
datasetNoY = np.loadtxt(f, delimiter=',', usecols=(0,2), unpack=False)

f = open(filepath)
dataset3d = np.loadtxt(f, delimiter=',', usecols=(0,1,2), unpack=False)
plot = 221
plt.subplot(plot)
plt.scatter(dataset[:,0], dataset[:,1])
plt.savefig('dataset.png')

hdbscan_ = hdbscan.HDBSCAN(min_cluster_size=6, min_samples=4)
#run hdbscan and find the number of clusters
#if clusters > 2, rerun hdbscan on that
#if cluters == 2 (that's what we want!) then be done
# the two clusers should be the two lanelines
start_time = time.time()
hdbscan_.fit(dataset3d)
time_taken = time.time() - start_time
labels = hdbscan_.labels_
print('data-specific time hdbscan: ', time_taken)
plt.subplot(222)
#plt.scatter(dataset[:,0], dataset[:,1], c=labels)
#

# threshold = pd.Series(hdbscan_.outlier_scores_).quantile(0.48)
# outliers = np.where(hdbscan_.outlier_scores_ > threshold)[0] #used for outlierN
outliers = np.where(hdbscan_.labels_ == -1) #used for outlierSurvey
plt.scatter(dataset[:,0], dataset[:,1], c='gray')
plt.scatter(dataset[outliers, 0], dataset[outliers, 1], c='red')
plt.savefig('outlierSurvey.png')

newDataset = np.delete(dataset3d, np.where(hdbscan_.labels_ == -1), 0)

hdbscan_ = hdbscan.HDBSCAN(min_cluster_size=4, min_samples=6)
hdbscan_.fit(newDataset)
labels = hdbscan_.labels_
plt.subplot(223)
plt.scatter(newDataset[:,0], newDataset[:,1], c=labels)
# sns.distplot(hdbscan_.outlier_scores_[np.isfinite(hdbscan_.outlier_scores_)], rug=True)
plt.subplot(224)
outliers = np.where(hdbscan_.labels_ == -1) #used for outlierSurvey
plt.scatter(newDataset[:,0], newDataset[:,1], c='gray')
plt.scatter(newDataset[outliers, 0], newDataset[outliers, 1], c='red')
import pdb; pdb.set_trace()

# savefile = "hdbNoise"
# for min_clust in range(2,9):
#     for min_samp in range(1,9):
#         hdbscan_ = hdbscan.HDBSCAN(min_cluster_size=min_clust, min_samples=min_samp)
#         start_time = time.time()
#         hdbscan_.fit(dataset3d)
#         time_taken = time.time() - start_time
#         labels = hdbscan_.labels_
#         print('data-specific time hdbscan: ', time_taken)
#         print(plot)
#         plt.subplot(222)
#         plt.scatter(dataset[:,0], dataset[:,1], c=labels)
#         thisSave = savefile + str(min_clust) + str(min_samp)
#         plt.savefig(thisSave)
