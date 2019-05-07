#!/usr/bin/env python
# coding: utf-8

# In[4]:


# usage: $python3 CULane_Remake.py [--no-vis]
# [--no-vis] for not showing frame, stopping after each frame


# ## Imports

# In[5]:


#from shutil import copy
#from shutil import copyfile
import os
import sys
import cv2 as cv2
import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join
from os import walk
#import IPython
#IPython.embed() # to debug in notebook
#import random
print("Successfully imported all")


# ## Convert notebook to python file

# In[6]:


#!jupyter nbconvert --to script CULane_Remake.ipynb
# Can use the command line argument instead:
#$jupyter nbconvert --to script CULane_Remake.ipynb


# ## Print pretty with colors

# In[7]:


# Credit: https://stackoverflow.com/questions/287871/print-in-terminal-with-colors
class CMD_C:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'   # End formatting
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


# ## Helper functions

# ### Parse Command Args, other setup
# 

# In[8]:


display_results = False if sys.argv[-1] == "--no-vis" else True
print("Display results? = ", display_results)


# In[9]:


cwd = os.getcwd()
print("CWD: ", cwd)


# ## Gather all images in the folders

# In[15]:


# Folders of annotations
root_dir = "/root/atlas_root/teams/team-1/Datasets/CULane_Dataset_RGB/"
img_folders = ["driver_100_30frame/", "driver_161_90frame/", "driver_182_30frame/", "driver_193_90frame/", "driver_23_30frame/", "driver_37_30frame/"]
folders = [root_dir + f for f in img_folders]
annotation_paths = []
# Credit: https://www.mkyong.com/python/python-how-to-list-all-files-in-a-directory/
for folder in folders:
    for r, d, files in os.walk(folder):
        for file in files:
            file_path = os.path.join(r, file)
            if file_path.split("/")[-1].split(".")[-1] == "jpg":
                annotation_paths.append(file_path)

print("Number of annotations total: ", CMD_C.OKBLUE, len(annotation_paths), CMD_C.ENDC, sep="")
for i in range(0,3):
    print(annotation_paths[i])


# ## Main loop

# In[34]:


plt.rcParams["figure.figsize"] = (20,10)

# CALCULATE MEAN OF ALL REAL IMAGES
r_avg_total = 0   # Total of the average of each image's red channel
g_avg_total = 0   # ^
b_avg_total = 0   # ^

#img_width = annotation_paths[0].shape()
for i, annotation_path in enumerate(annotation_paths):
    print("Processing image ", CMD_C.OKGREEN, i, CMD_C.ENDC, " / ", len(annotation_paths), " : ", sep="", end="")

    # Read image
    img = cv2.imread(annotation_path, cv2.IMREAD_COLOR)
    print("read ", sep="", end="\n")
    
    #plt.imshow(img[:,:,0], cmap=None)
    #print(img.shape)
    
    # Add the r,g,b
    totals = np.sum(img)
    total_r = np.sum(img[:, :, 0])
    total_g = np.sum(img[:, :, 1])
    total_b = np.sum(img[:, :, 2])
    #print("\ntotals for 1 image: ", totals)
    #print("total r for 1 image: ", total_r)
    #print("total g for 1 image: ", total_g)
    #print("total b for 1 image: ", total_b)
    #break
    
    r_avg_total += total_r / (img.shape[0] * img.shape[1])
    g_avg_total += total_g / (img.shape[0] * img.shape[1])
    b_avg_total += total_b / (img.shape[0] * img.shape[1])
        
avg_r = r_avg_total / len(annotation_paths)
avg_g = g_avg_total / len(annotation_paths)
avg_b = b_avg_total / len(annotation_paths)

print("avg_r (channel 0) = ", avg_r)
print("avg_g (channel 1)= ", avg_g)
print("avg_b (channel 2)= ", avg_b)
print("Done!")


# In[ ]:




