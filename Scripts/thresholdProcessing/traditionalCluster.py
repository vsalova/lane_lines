import numpy as np
import cv2
import matplotlib.pyplot as plt
import time
import hdbscan
import sklearn.cluster
import sklearn.linear_model
from sklearn import svm

def findCentroid(cluster):
    sumX = 0
    sumY = 0

    for i in cluster:
        #import pdb; pdb.set_trace()
        sumX += i[0]
        sumY += i[1]
    centroid = [sumX/len(cluster), sumY/len(cluster)]
    return centroid

def findTopXVal(cluster):
    maxY = 0
    xVals = []
    xAvg = 0
    for i in cluster:
        pointX = i[0]
        pointY = i[1]
        if(maxY > pointY):
            continue
        elif(pointY > maxY):
            maxY = pointY
            xVals.clear()
            xVals.append(pointX)
        else: #pointY = maxY
            xVals.append(pointX)
    for i in xVals:
        xAvg += i
    return [xAvg / len(xVals), maxY]

def groupPoints(numClusters, labels, data):
    #This function will ultimately return centriod and topXVals for each cluster
    clusters = [None]*numClusters
    centroid = [None]*numClusters
    topXVal = [None]*numClusters
    for i in range(0,len(data[0])):
        label = labels[i]
        if(label < 0):
            continue
        point = [data[0][i], data[1][i]]
        if(clusters[label] == None):
            clusters[label] = [point]
        else:
            clusters[label].append(point)

    for i in range(0, numClusters):
        centroid[i] = findCentroid(clusters[i])
        topXVal[i] = findTopXVal(clusters[i])
    return centroid, topXVal

def groupBasic(numClusters, labels, data):
    clusters = [None]*numClusters
    label = 0
    #import pdb; pdb.set_trace()
    for i in range(0,len(data[0])):

        label = labels[i]
        #import pdb; pdb.set_trace()
        point = [data[0][i], data[1][i]]
        if(clusters[label] == None):
            clusters[label] = [point]
        else:
            clusters[label].append(point)
    return clusters

def cluster(filepath):
    imgNice = cv2.imread(filepath, 0)

    whiteNice = cv2.findNonZero(imgNice).transpose()
    alpha = whiteNice[:][0][0]
    beta = whiteNice[:][1][0]
    whiteNice = np.ones((2, alpha.shape[0]))
    whiteNice[0] = alpha
    whiteNice[1] = beta
    #whiteNice = whiteNice.transpose()
    plt.figure(figsize=(12,12))

    plt.subplot(321)
    plt.scatter(whiteNice[0], whiteNice[1])

    hdbscan_ = hdbscan.HDBSCAN(min_cluster_size=6, min_samples=4)

    start_time = time.time()
    hdbscan_.fit(whiteNice.transpose())
    time_taken = time.time() - start_time
    labels = hdbscan_.labels_
    print('data-specific time hdbscan: ', time_taken)
    plt.subplot(322)
    plt.scatter(whiteNice[0], whiteNice[1], c=labels)

    centroid, xVal = groupPoints(hdbscan_.labels_.max() + 1, labels, whiteNice)
    #import pdb; pdb.set_trace()

    newPoints = centroid + xVal
    x = []
    y = []
    for i in newPoints:
        x.append(i[0])
        y.append(i[1])

    z = np.array([[300,400], [1100, 400]], np.int32)
    KMeans = sklearn.cluster.KMeans(n_clusters=2, init=z)
    KMeans.fit_predict(newPoints)

    plt.subplot(323)
    plt.scatter(x, y, c=KMeans.labels_)
    #plt.show()

    clusters = groupBasic(2, KMeans.labels_, [x]+[y])
    x1 = []
    y1 = []
    x2 = []
    y2 = []

    xy1 = clusters[0]
    xy2 = clusters[1]
    for i in xy1:
            x1.append(i[0])
            y1.append(i[1])
    for i in xy2:
            x2.append(i[0])
            y2.append(i[1])

    # clf = svm.SVC(kernel='linear', C = 1.0)
    # clf.fit(x1,y1)
    # import pdb; pdb.set_trace()
    x1 = np.asarray(x1)
    x2 = np.asarray(x2)
    y1 = np.asarray(y1)
    y2 = np.asarray(y2)

    linreg1 = sklearn.linear_model.LinearRegression().fit(x1.reshape((-1,1)),y1)
    linreg2 = sklearn.linear_model.LinearRegression().fit(x2.reshape((-1,1)),y2)
    spaceleft = np.linspace(300,400,100)
    spaceright = np.linspace(900,1000, 100)
    plt.plot(spaceleft, linreg1.predict(spaceleft[:, np.newaxis]))
    plt.plot(spaceright, linreg2.predict(spaceright[:, np.newaxis]))

    plt.xlim(250,1150)
    plt.ylim(-50,750)
    #import pdb; pdb.set_trace()
    plt.show()
    plt.savefig('final.png')
    #import pdb; pdb.set_trace()

# cv2.imshow('image', whiteNice)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

filepath = "thresholdNice.png"
cluster(filepath)
