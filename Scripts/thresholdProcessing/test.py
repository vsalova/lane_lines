import numpy as np
import cv2
import matplotlib.pyplot as plt
import time
import hdbscan
import sklearn.cluster
import sklearn.linear_model

x2 = [962.18913043, 959.73333333]
x2 = np.asarray(x2)

y2 = [700.2673913 , 566.96]
y2 = np.asarray(y2)



linreg2 = sklearn.linear_model.LinearRegression().fit(x2.reshape((-1,1)),y2)
spaceright = np.linspace(900,1000, 100)
plt.plot(spaceright, linreg2.predict(spaceright[:, np.newaxis]))
plt.scatter(x2, y2)
plt.xlim(250,1150)
plt.ylim(-50,750)
plt.show()
