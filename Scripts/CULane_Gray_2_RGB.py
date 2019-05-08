#!/usr/bin/env python
# coding: utf-8

# In[1]:


# usage: $python3 CULane_Remake.py [--no-vis]
# [--no-vis] for not showing frame, stopping after each frame


# ## Imports

# In[3]:


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

# In[4]:


#!jupyter nbconvert --to script CULane_Remake.ipynb
# Can use the command line argument instead:
#$jupyter nbconvert --to script CULane_Remake.ipynb


# ## Print pretty with colors

# In[5]:


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

# In[6]:


display_results = False if sys.argv[-1] == "--no-vis" else True
print("Display results? = ", display_results)


# In[7]:


cwd = os.getcwd()
print("CWD: ", cwd)


# ## Gather all images in the folders

# In[8]:


# Folders of annotations
folder_dir = "/root/atlas_root/teams/team-1/Datasets/CULane_Dataset_RGB/laneseg_label_w16/"
annotation_paths = []
# Credit: https://www.mkyong.com/python/python-how-to-list-all-files-in-a-directory/
for r, d, files in os.walk(folder_dir):
    for file in files:
        annotation_paths.append(os.path.join(r, file))

print("Number of annotations total: ", CMD_C.OKBLUE, len(annotation_paths), CMD_C.ENDC, sep="")


# ## Main loop

# In[12]:


r_ind = 43456  # A random curvy section

for i, annotation_path in enumerate(annotation_paths):
    print("Processing image ", CMD_C.OKGREEN, i, CMD_C.ENDC, " / ", len(annotation_paths), " : ", sep="", end="")

    # Read image
    img = cv2.imread(annotation_path, cv2.IMREAD_GRAYSCALE)
    print("read ", sep="", end="")
    
    # Save new image (as PNG)
    save_success = cv2.imwrite(annotation_path, cv2.cvtColor(img, cv2.COLOR_GRAY2RGB))
    if save_success:
        print("saved to ", annotation_path, sep="")    # Replace old image
    else:
        print(CMD_C.FAIL, "FAILED TO SAVE IMAGE", CMD_C.ENDC, annotation_path)
        break


# In[ ]:


print("All done!")

