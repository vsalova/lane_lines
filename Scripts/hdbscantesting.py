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

filepath = "hough_example_output_noisy.txt"

f = open(filepath)
dataset = np.loadtxt(f, delimiter=',', usecols=(0,1), unpack=False)

f = open(filepath)
datasetNoY = np.loadtxt(f, delimiter=',', usecols=(0,2), unpack=False)

f = open(filepath)
dataset3d = np.loadtxt(f, delimiter=',', usecols=(0,1,2), unpack=False)

hdbscan_ = hdbscan.HDBSCAN()
hdbscan_data = benchmark_algorithm(dataset_sizes, hdbscan_.fit, (), {})
print('finished hdbscan')
start_time = time.time()
hdbscan_.fit(dataset3d)
time_taken = time.time() - start_time
labels = hdbscan_.labels_
print('data-specific time hdbscan: ', time_taken)
plt.subplot(222)
plt.scatter(dataset[:,0], dataset[:,1], c=labels)
plt.savefig('hdbNoise.png')
