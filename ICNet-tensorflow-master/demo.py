#!/usr/bin/env python
# coding: utf-8

# In[2]:


import argparse
import tensorflow as tf
import numpy as np
import cv2
import time
import matplotlib.pyplot as plt

from tqdm import trange
from utils.config import Config
from model import ICNet, ICNet_BN


# # Setup configurations

# In[3]:


model_config = {'train': ICNet, 'trainval': ICNet, 'train_bn': ICNet_BN, 'trainval_bn': ICNet_BN, 'others': ICNet_BN}

# Choose dataset here, but remember to use `script/downlaod_weight.py` first
dataset = 'culane'
filter_scale = 1
    
class InferenceConfig(Config):
    def __init__(self, dataset, is_training, filter_scale):
        Config.__init__(self, dataset, is_training, filter_scale)
    
    # You can choose different model here, see "model_config" dictionary. If you choose "others", 
    # it means that you use self-trained model, you need to change "filter_scale" to 2.
    # model_type = 'trainval'
    model_type = 'others'

    # Set pre-trained weights here (You can download weight from Google Drive) 
    #model_weight = './model/cityscapes/icnet_cityscapes_trainval_90k.npy'
    model_weight = './snapshots/model.ckpt-60000'
    
    # Define default input size here
    INFER_SIZE = (1024, 2048, 3)
                  
cfg = InferenceConfig(dataset, is_training=False, filter_scale=filter_scale)
#cfg.display()


# # Create graph, session, and restore weights

# In[4]:


# Create graph here 
model = model_config[cfg.model_type]
net = model(cfg=cfg, mode='inference')

# Create session & restore weights!
net.create_session()
net.restore(cfg.model_weight)


# # Run segmentation on single image

# In[ ]:


im1 = cv2.imread('./data/input/cityscapes1.png')

if im1.shape != cfg.INFER_SIZE:
    im1 = cv2.resize(im1, (cfg.INFER_SIZE[1], cfg.INFER_SIZE[0]))

results1 = net.predict(im1)
overlap_results1 = 0.5 * im1 + 0.5 * results1[0]
vis_im1 = np.concatenate([im1/255.0, results1[0]/255.0, overlap_results1/255.0], axis=1)

plt.figure(figsize=(20, 15))
plt.imshow(vis_im1)


# In[ ]:


im2 = cv2.imread('./data/input/cityscapes2.png')
results2 = net.predict(im2)

im2 = cv2.cvtColor(im2, cv2.COLOR_BGR2RGB)
overlap_results2 = 0.5 * im2 + 0.5 * results2[0]

vis_im2 = np.concatenate([im2/255.0, results2[0]/255.0, overlap_results2/255.0], axis=1)

plt.figure(figsize=(20, 15))
plt.imshow(vis_im2)


# # Test inference speed

# In[ ]:


elapsed_times = []

for i in range(50):
    start_t = time.time()
    
    _ = net.predict(im1)
    
    duration = time.time() - start_t
    
    if i > 0:
        elapsed_times.append(duration)

print('Average time: {:.4f}, about {:.6f} fps'.format(np.mean(elapsed_times), 1/np.mean(elapsed_times)))


# In[ ]:




