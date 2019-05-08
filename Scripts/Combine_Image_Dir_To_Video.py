#!/usr/bin/env python
# coding: utf-8

# In[2]:


#from shutil import copy
#from shutil import copyfile
import os
import sys
import cv2 as cv2
import glob
#import numpy as np
#import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join
from os import walk
import IPython
#IPython.embed() # to debug in notebook
import random
print("Successfully imported all")


# In[4]:


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
    


# # INPUT ARGS

# In[6]:


input_img_dir = sys.argv[-2]
print("Source image directory: ", input_img_dir)

if os.path.exists(input_img_dir) == False:
    print(CMD_C.FAIL, input_img_dir, CMD_C.ENDC, " DOESN'T EXISTS", sep="")
    assert False    # Because jupyter notebook doesn't like quit()

save_as_name = sys.argv[-1]
print("Saving combined video as: ", save_as_name)
cwd = os.getcwd()
print("CWD: ", cwd)

save_as_name = cwd + "/" + save_as_name
print("Path to save combined video: ", save_as_name)


# # GATHER PICTURES

# In[ ]:


def acceptable_file(file_path):
    ext = file_path.split("/")[-1].split(".")[-1]
    ext = ext.lower()
    accepted = ["png", "jpg", "jpeg"]
    return ext in accepted

picture_paths = glob.glob(input_img_dir + "/*")     # Collect all files
print("File in picture path: ", len(picture_paths))
picture_paths_passed = [p for p in picture_paths if acceptable_file(p)]
num_files_removed = len(picture_paths) - len(picture_paths_passed)
if num_files_removed > 0:
    print(CMD_C.FAIL, num_files_removed, CMD_C.ENDC, " FILES NOT INCLUDED SINCE THEY AREN'T png, jpg, or jpeg", sep="")
picture_paths = picture_paths_passed
if len(picture_paths) == 0:
    print("There are images, ending")
    assert False

# Make sure images are in order?


# # COMBINE PICTURES

# In[1]:


first_img = cv2.imread(picture_paths[-1])
height, width, layers =  first_img.shape

video_writer = cv2.VideoWriter(save_as_name, -1, 1, (width, height))

for i, pp in enumerate(picture_paths):
    img = cv2.imread(pp)
    video_writer.write(img)
    print("Wrote frame ", CMD_C.OKGREEN, str(i), CMD_C.ENDC, " / ", str(len(picture_paths)))

cv2.destroyAllWindows()
video_writer.release()


# In[ ]:


print("All done")

