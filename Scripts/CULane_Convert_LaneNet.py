#!/usr/bin/env python
# coding: utf-8

# In[1]:


# usage: $python3 CULane_Convert_LaneNet.py
# Will save converted CULANE into the same location that this script is run.


# ## Imports

# In[2]:


from shutil import copy
from shutil import copyfile
import os
import sys
import cv2 as cv2
import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join
from os import walk
import IPython
#IPython.embed() # to debug in notebook
import random
print("Successfully imported all")


# ## Constants and paths

# In[3]:


cwd = os.getcwd() + "/"
print("CWD: ", cwd)


# In[4]:


display_results = False if sys.argv[-1] == "--no-vis" else True


# In[5]:


export_folder_name = cwd + "culane_dataset_lanenet/"
print("Export folder: ", export_folder_name)

export_img_dir = export_folder_name + "image/"
export_instance_dir = export_folder_name + "gt_image_instance/"
export_binary_dir = export_folder_name + "gt_image_binary/"

# CHANGE THIS TO THE LOCATION OF YOUR LANENET DATA FOLDER
base_export_dir = "/root/root_dit_atlas/home/cjcramer/lane_lines/LaneNet/data/"
print("base_export_dir: ", base_export_dir)

image_dir = base_export_dir + "image/"
instance_dir = base_export_dir + "gt_image_instance/"
binary_dir = base_export_dir + "gt_image_binary/"
validation_path = base_export_dir + "val.txt"
train_path = base_export_dir + "train.txt"

# CHANGE THIS TO THE LOCATION OF YOUR CULANE DATASET
data_base_dir = "/root/atlas_root/teams/team-1/Datasets/CULane_Dataset/"
print("CULane path: ", data_base_dir)

# Folders of annotations
annotation_dir = data_base_dir + "laneseg_label_w16/"

# Take all images from annotation directory, get corresponding actual image from CULane,
# convert the image into annotations with values 20, 70, 120, 170
# convert the image to all white annotations


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

# In[8]:


def make_dir(dir_path):
    if (os.path.exists(dir_path)):
        print(dir_path, CMD_C.OKGREEN, " already exists in the current working direcrectory: ", CMD_C.ENDC, cwd, sep="")
    else:
        try:
            os.mkdir(dir_path)
        except OSError:
            print(FAIL, "Could not create destination folder: ", dir_path, ENDC)
            return False
        else:
            print("Sucessfully made destination folder: ", dir_path)
    return True


# # Gather all file paths

# In[9]:


# Credit: https://www.mkyong.com/python/python-how-to-list-all-files-in-a-directory/
annotation_paths = []
for r, d, files in os.walk(annotation_dir):
    for file in files:
        annotation_paths.append(os.path.join(r, file))

print("Number of annotations total: ", CMD_C.OKBLUE, len(annotation_paths), CMD_C.ENDC, sep="")


# # Gather corresponding actual pictures 

# In[10]:


paths = []   # CULane img, CULane annot, In LaneNet img, in LaneNet binary, in LaneNet inst, copy to img, copy to binary, copy to inst 
new_file_name_counter = 0      # Since CULane has many files with the same name in different directories, enumerate the pictures.

for annot_path in annotation_paths:
    tok = annot_path.split("/")
    filename = tok[-1]
    filename_no_ext = filename.split(".")[0]
    path = "/".join(tok[-3:-1])
    path_to_file = data_base_dir + path + "/" + filename_no_ext + ".jpg"
    # Check to see if that file actually exists, because not all annotated pictures have pitures?
    # Or I deleted some pictures?
    # Should /CULane_Dataset/driver_161_90frame/06031716_0888.MP4/ be empty?
    if os.path.isfile(path_to_file) == False:
        continue
    
    image_path_lanenet = image_dir + str(new_file_name_counter) + ".png"
    binary_path_lanenet = binary_dir + str(new_file_name_counter) + ".png"
    instance_path_lanenet = instance_dir + str(new_file_name_counter) + ".png"
   
    export_image_path_lanenet = export_img_dir + str(new_file_name_counter) + ".png"
    export_binary_path_lanenet = export_binary_dir + str(new_file_name_counter) + ".png"
    export_instance_path_lanenet = export_instance_dir + str(new_file_name_counter) + ".png"
    
    paths.append((path_to_file, annot_path, image_path_lanenet, binary_path_lanenet, instance_path_lanenet, export_image_path_lanenet, export_binary_path_lanenet, export_instance_path_lanenet))
    new_file_name_counter += 1
    
    
print("Example paths")
for i in range(0, 2):
    print(paths[i][0])
    print(paths[i][1])
    print(paths[i][2])
    print(paths[i][3])
    print(paths[i][4])
    print(paths[i][5])
    print(paths[i][6])
    print(paths[i][7], "\n")


# # Make destination folder to save all outputs to

# In[11]:


folders_to_create = [export_folder_name, export_folder_name + "image/", export_folder_name + "gt_image_instance/", export_folder_name + "gt_image_binary/"]
for folder in folders_to_create:
    if make_dir(folder) == False:
        print(CMD_C.FAIL, "COULD NOT CREATE THE FOLDER: ", folder, CMD_C.ENDC, sep="")


# # Save train.txt and val.txt

# In[12]:


# 0.9 to 0.1 train val split
with open(export_folder_name + "train.txt", "w") as train_file:
    with open(export_folder_name + "val.txt", "w") as val_file:
        for _, _, img_path, bin_path, inst_path, _, _, _ in paths:
            str_to_write = img_path + " " + bin_path + " " + inst_path + "\n"
            if random.random() < 0.9:
                train_file.write(str_to_write)
            else:
                val_file.write(str_to_write)
print("Wrote train.txt and val.txt")


# ## Main loop

# In[13]:


plt.rcParams["figure.figsize"] = (20,10)
#r_ind = 43456  # A random curvy section
file_no = 0
for CU_img_path, CU_annot_path, _, _, _, export_img_path, export_bin_path, export_inst_path in paths: 
    #filename_no_ext = CU_img_path.split("/")[-1].split(".")[0]
    file_no += 1
    print("Processing image ", CMD_C.OKGREEN, file_no, CMD_C.ENDC, " / ", len(paths), " : ", sep="", end="")
    print(CMD_C.OKGREEN,  "{0:.1f}".format(100 * file_no / len(paths)), "% | ", CMD_C.ENDC, sep="", end="")
    
    # Copy the real image to the image/ folder
    try:
        # Check if image already exists, and we will overwrite it
        if os.path.isfile(export_img_path) == True:
            print(CMD_C.FAIL, export_img_path, " already exists, DO NOT WANT TO OVERWRITE IT SO SKIPPING", CMD_C.ENDC)
            continue
        img = cv2.imread(CU_img_path, cv2.IMREAD_COLOR)
        save_success = cv2.imwrite(export_img_path, img)
        if save_success == False:
            print(CMD_C.FAIL, "Could not copy file: ", culane_pict_path, CMD_C.ENDC, " to ", CMD_C.FAIL, img_copy_dest, CMD_C.ENDC, sep="", end="")
    except:
        print(CMD_C.FAIL, "Could not copy file: ", culane_pict_path, CMD_C.ENDC, " to ", CMD_C.FAIL, img_copy_dest, CMD_C.ENDC, sep="", end="")
    else:
        print(" copied img | ", sep="", end="")
    
    #IPython.embed() # to debug in notebook
    #print("Exiting")
    #break
    
    # Read in the annotation image from CULane
    try:
        img = cv2.imread(CU_annot_path, cv2.IMREAD_GRAYSCALE)
        # Convert it to all white
        ret, white = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY)
        # Convert the 1,2,3,4 annotated pixels to 20 70 120 170 respectively
        change_scale = img.copy()
        change_to = {1:20, 2:70, 3:120, 4:170}
        #print(change_scale)
        for old, new in change_to.items():
            change_scale[change_scale == old] = new
            #change_scale[np.where((change_scale == [1]).all(axis = 0))] = [20]
        #print(change_scale)
        # Save those two images
    except Exception as ex:
        print("Error", ex)
    else:
        pass
    
    # Save white
    save_success = cv2.imwrite(export_bin_path, white)
    if save_success:
        print("binary saved | ", sep="", end = "")
    else:
        print(CMD_C.FAIL, "FAILED TO SAVE IMAGE", CMD_C.ENDC, export_bin_path)
    
    # Save change_scale
    save_success = cv2.imwrite(export_inst_path, change_scale)
    if save_success:
        print("annotation saved | ", sep="", end = "")
    else:
        print(CMD_C.FAIL, "FAILED TO SAVE IMAGE", CMD_C.ENDC, export_inst_path)
    
    # Show result for first frame if display results
    if display_results == True:
        f, axs = plt.subplots(1, 3)
        for _, ax in np.ndenumerate(axs):   # Remove axes
            ax.axis('off')
        axs[0].imshow(img)
        axs[1].imshow(white)
        axs[2].imshow(change_scale)
        axs[0].set_title("Original (false color)")
        axs[1].set_title("Annotations all white")
        axs[2].set_title("Change Scale")
        plt.tight_layout()
        plt.show()
        break
    print()


# In[14]:


print("All done!")

