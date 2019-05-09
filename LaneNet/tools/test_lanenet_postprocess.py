#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# @Time    : 18-5-23 上午11:33
# @Author  : Luo Yao
# @Site    : https://github.com/MaybeShewill-CV/lanenet-lane-detection
# @File    : test_lanenet.py
# @IDE: PyCharm Community Edition
"""
测试LaneNet模型
"""
import os
import os.path as ops
import argparse
import time
import math
import sys

#sys.path = ['', '/root/root_dit_atlas/home/cjcramer/lane_lines/LaneNet', '/root/anaconda3/lib/python36.zip', '/root/anaconda3/lib/python3.6', '/root/anaconda3/lib/python3.6/lib-dynload', '/root/anaconda3/lib/python3.6/site-packages']
import tensorflow as tf
import glob
import glog as log
import numpy as np
import matplotlib.pyplot as plt
import cv2

from lanenet_model import lanenet_merge_model
from lanenet_model import lanenet_cluster
from lanenet_model import lanenet_postprocess
from config import global_config

CFG = global_config.cfg
VGG_MEAN = [103.939, 116.779, 123.68]

DO_POST_PROCESS = True

# Need more points, because poly is overfitting
ORDER = 3
Y_POLY_POINT_STEP = 1
EXTEND_LANE_DOWN_BY = 30

lane_colors = [[255, 0, 0], [0, 255, 0], [0, 0, 255], [0, 255, 255], [255, 255, 0], [255, 0, 255], [255, 255, 255]]


# HSV ranges (THESE ARE HSV VALUES)
lower_blu = np.array([118, 240, 10]) 
upper_blu = np.array([122, 255, 255])

lower_red = np.array([0, 240, 10]) 
upper_red = np.array([6, 255, 255])

lower_grn = np.array([54, 240, 10]) 
upper_grn = np.array([66, 255, 255])

lower_cya = np.array([84, 240, 10]) 
upper_cya = np.array([96, 255, 132])


# Function of lane width varied over height
# May need to fidget with this if the resolution differernt than CULane
def lane_width_px(y):
    if y <= 260:
        return 1
    return (int) (y * 0.1 - 25)


# @each row in @points should be of form (x,y)
# @lane_number is the number to be placed in the grayscale image, which represents the class of the lane line
# @points should be going from the top of the image downwards, the drawing will stop when the lane line goes off screen
def draw_lane(img, points, lane_number):
    # Draw a line in for each point
    for i in range(points.shape[0]):
        point = points[i, :]
        lane_width = lane_width_px(point[1])
        if point[1] < 0 or point[1] > img.shape[0] or point[0] < lane_width // 2 or point[0] > img.shape[1] + lane_width // 2:
            return
        cv2.line(img, (point[0] - lane_width // 2, point[1]), (point[0] + lane_width // 2, point[1]), lane_number, 1) # Thickness = 1


# @point should be (x,y) form of point, top left corner is origin point
# @ width, height should be the height and width of the frame
def angle_from_top_center(point, width, height):
    dx = (width / 2) - point[0]
    dy = point[1] - 0.25 * height   # Take center point y dimension to be closer to vanishing point
    return math.atan2(dy, dx)


def post_process_output(mask):
    # Threshold different color lines out
    mask_hsv = cv2.cvtColor(mask, cv2.COLOR_BGR2HSV)    # Convert to HSV
    # Get each lane
    red_res = cv2.inRange(mask_hsv, lower_red, upper_red)
    blu_res = cv2.inRange(mask_hsv, lower_blu, upper_blu)  
    grn_res = cv2.inRange(mask_hsv, lower_grn, upper_grn)  
    cya_res = cv2.inRange(mask_hsv, lower_cya, upper_cya)  
    
    lanes = [{"color": "red", "lane": red_res}, {"color": "blue", "lane": blu_res}, 
             {"color": "green", "lane": grn_res}, {"color": "cyan", "lane": cya_res}]

    # Get centers first
    for i in range(len(lanes)):
        lane = lanes[i]["lane"]
        M = cv2.moments(lane)
        # calculate x,y coordinate of center
        if M["m00"] == 0:
                lanes[i]["center"] = None
                lanes[i]["DNE"] = True
        else:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])
            #cv2.circle(lane, (cX, cY), 40, [255], -1)
            lanes[i]["center"] = (cX, cY)
    
    # Get angles
    for i in range(len(lanes)):
        if "DNE" not in lanes[i].keys():
            lanes[i]["angle"] = angle_from_top_center(lanes[i]["center"], mask.shape[1], mask.shape[0])
        else:
            lanes[i]["angle"] = 0    # Assume left most lane
    
    # Sort the lanes according to the angle between the centers of each lane and the top center
    def sort_by_angle(d):
        return d["angle"]
    lanes = sorted(lanes, key = sort_by_angle)
        
    post_processed = np.zeros((lanes[0]["lane"].shape[0], lanes[0]["lane"].shape[1], 3), np.uint8)
    # Calculate 6th order polynomial for each
    for i in range(len(lanes)):
        if "DNE" in lanes[i].keys():
            continue
        points = np.argwhere(lanes[i]["lane"] != 0)
        # Polynomial regression, Evaluate polynomial, Draw polynomial
        if points.shape[0] > 0:
            coeefs = np.polyfit(points[:,0], points[:,1], ORDER)  # As a function of Y
            y_vals = list(range(min(points[:,0]), max(points[:,0]) + EXTEND_LANE_DOWN_BY, Y_POLY_POINT_STEP))
            x_vals = [np.polyval(coeefs, y) for y in y_vals]
            poly_points = np.int32( np.column_stack((x_vals, y_vals)) )
            # Draw the polynomial
            draw_lane(post_processed, poly_points, lane_colors[i])
    
    #mask = cv2.cvtColor(mask, cv2.COLOR_BGR2RGB)   # So we can see it properly when plotting
    #cv2.circle(mask, (mask.shape[1] // 2, int(mask.shape[0] * 0.25)), 20, [255], -1)
    
    #plt.rcParams["figure.figsize"] = (20,10)
    #f, axs = plt.subplots(3, 2)
    #for _, ax in np.ndenumerate(axs):   # Remove axes
    #    ax.axis('off')
    #axs[0][0].imshow(mask)
    #axs[0][1].imshow(post_processed)
    #axs[1][0].imshow(blu_res, cmap='gray')
    #axs[1][1].imshow(red_res, cmap='gray')
    #axs[2][0].imshow(grn_res, cmap='gray')
    #axs[2][1].imshow(cya_res, cmap='gray')
    #plt.tight_layout()
    #plt.show()
    return post_processed


def minmax_scale(input_arr):
    """

    :param input_arr:
    :return:
    """
    min_val = np.min(input_arr)
    max_val = np.max(input_arr)

    output_arr = (input_arr - min_val) * 255.0 / (max_val - min_val)

    return output_arr


def test_lanenet_batch(image_dir, weights_path, batch_size, use_gpu, save_dir=None):
    """

    :param image_dir:
    :param weights_path:
    :param batch_size:
    :param use_gpu:
    :param save_dir:
    :return:
    """

    tf.reset_default_graph()    # So we can run this multiple times

    print("save_dir in function: ", save_dir)

    assert ops.exists(image_dir), '{:s} not exist'.format(image_dir)

    log.info('开始获取图像文件路径...')
    image_path_list = glob.glob('{:s}/**/*.jpg'.format(image_dir), recursive=True) + \
                      glob.glob('{:s}/**/*.png'.format(image_dir), recursive=True) + \
                      glob.glob('{:s}/**/*.jpeg'.format(image_dir), recursive=True)

    print("NUMBER OF IMAGES IN IMAGE_PATH: ", len(image_path_list))

    input_tensor = tf.placeholder(dtype=tf.float32, shape=[None, 256, 512, 3], name='input_tensor')
    phase_tensor = tf.constant('test', tf.string)

    net = lanenet_merge_model.LaneNet(phase=phase_tensor, net_flag='vgg')
    binary_seg_ret, instance_seg_ret = net.inference(input_tensor=input_tensor, name='lanenet_model')

    cluster = lanenet_cluster.LaneNetCluster()
    postprocessor = lanenet_postprocess.LaneNetPoseProcessor()

    saver = tf.train.Saver()

    # Set sess configuration
    if use_gpu:
        sess_config = tf.ConfigProto(device_count={'GPU': 1})
    else:
        sess_config = tf.ConfigProto(device_count={'GPU': 0})
    sess_config.gpu_options.per_process_gpu_memory_fraction = CFG.TEST.GPU_MEMORY_FRACTION
    sess_config.gpu_options.allow_growth = CFG.TRAIN.TF_ALLOW_GROWTH
    sess_config.gpu_options.allocator_type = 'BFC'

    sess = tf.Session(config=sess_config)

    with sess.as_default():

        saver.restore(sess=sess, save_path=weights_path)

        print("SUCCESSFULLY RESTORED FROM SESSION")

        epoch_nums = int(math.ceil(len(image_path_list) / batch_size))

        print("EPOCH_NUMS = ", epoch_nums)

        for epoch in range(epoch_nums):
            log.info('[Epoch:{:d}] 开始图像读取和预处理...'.format(epoch))
            t_start = time.time()
            image_path_epoch = image_path_list[epoch * batch_size:(epoch + 1) * batch_size]
            image_list_epoch = [cv2.imread(tmp, cv2.IMREAD_COLOR) for tmp in image_path_epoch]
            image_vis_list = image_list_epoch
            image_list_epoch = [cv2.resize(tmp, (512, 256), interpolation=cv2.INTER_LINEAR)
                                for tmp in image_list_epoch]
            image_list_epoch = [tmp - VGG_MEAN for tmp in image_list_epoch]
            t_cost = time.time() - t_start
            log.info('[Epoch:{:d}] 预处理{:d}张图像, 共耗时: {:.5f}s, 平均每张耗时: {:.5f}'.format(
                epoch, len(image_path_epoch), t_cost, t_cost / len(image_path_epoch)))

            t_start = time.time()
            binary_seg_images, instance_seg_images = sess.run(
                [binary_seg_ret, instance_seg_ret], feed_dict={input_tensor: image_list_epoch})
            t_cost = time.time() - t_start
            log.info('[Epoch:{:d}] 预测{:d}张图像车道线, 共耗时: {:.5f}s, 平均每张耗时: {:.5f}s'.format(
                epoch, len(image_path_epoch), t_cost, t_cost / len(image_path_epoch)))

            cluster_time = []
            for index, binary_seg_image in enumerate(binary_seg_images):
                t_start = time.time()
                binary_seg_image = postprocessor.postprocess(binary_seg_image)
                mask_image = cluster.get_lane_mask(binary_seg_ret=binary_seg_image,
                                                   instance_seg_ret=instance_seg_images[index])
                cluster_time.append(time.time() - t_start)
                mask_image = cv2.resize(mask_image, (image_vis_list[index].shape[1],
                                                     image_vis_list[index].shape[0]),
                                        interpolation=cv2.INTER_LINEAR)

                if save_dir is None:
                    plt.ion()
                    plt.figure('mask_image')
                    plt.imshow(mask_image[:, :, (2, 1, 0)])
                    plt.figure('src_image')
                    plt.imshow(image_vis_list[index][:, :, (2, 1, 0)])
                    plt.pause(3.0)
                    plt.show()
                    plt.ioff()

                if save_dir is not None:
                    # Mask is the predicted lane line pixels with colors on a black background
                    if DO_POST_PROCESS == True:
                        mask_image = post_process_output(mask_image)
                    # After this line it will layer the lines on the actual image
                    # Comment it out to save just the predictions
                    mask_image = cv2.addWeighted(image_vis_list[index], 1.0, mask_image, 1.0, 0)
                    
                    image_name = ops.split(image_path_epoch[index])[1]
                    image_save_path = ops.join(save_dir, image_name)
                    cv2.imwrite(image_save_path, mask_image)
                    print("SAVED MASK IMAGE TO ", image_save_path)

            log.info('[Epoch:{:d}] 进行{:d}张图像车道线聚类, 共耗时: {:.5f}s, 平均每张耗时: {:.5f}'.format(
                epoch, len(image_path_epoch), np.sum(cluster_time), np.mean(cluster_time)))

    sess.close()

    return


if __name__ == '__main__':
    # init args
    #args = init_args()

    #path = "/root/root_dit_atlas/teams/team-1/Datasets/TuSimple/train_set/clips/0313-2/58980"
    #print("Path: ", path)
    #print("Path exists? ", os.path.exists(path))
    #print("Ending early test")
    #quit()

    # NORMALLY CALL ONCE LIKE THIS:
    #python3 tools/test_lanenet.py --is_batch True --batch_size 1 
    # --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 
    # --image_path /root/root_dit_atlas/teams/team-1/Datasets/TuSimple/train_set/clips/0313-2/58980 
    # --use_gpu 0 --save_dir output_frames/vid1

    # BUT WE WILL CALL MULTIPLE TIMES FOR MULTIPLE IMAGE PATHS
    base_dir = "/root/root_dit_atlas/teams/team-1/Datasets/TuSimple/train_set/clips/"
    img_dirs = ["0313-1/10000", "0313-1/10020", "0313-1/10040", "0313-1/10060", "0313-1/10080", "0313-1/10100", "0313-1/10120", "0313-1/10140", "0313-1/10160",
                "0313-1/13200", "0313-1/13220", "0313-1/13240", "0313-1/13260", "0313-1/13280", "0313-1/13300", "0313-1/13320", "0313-1/13340", "0313-1/13360",
                "0313-1/17400", "0313-1/17420", "0313-1/17440", "0313-1/17460", "0313-1/17480", "0313-1/17500", "0313-1/17520", "0313-1/17540", "0313-1/17560", 
                "0313-1/6000" , "0313-1/6020" , "0313-1/6040" , "0313-1/6060" , "0313-1/6080" , "0313-1/6100" , "0313-1/6120" , "0313-1/6140" , "0313-1/6160" ,
                "0313-2/32500", "0313-2/32520", "0313-2/32540", "0313-2/32560", "0313-2/32580", "0313-2/32600", "0313-2/32620", "0313-2/32640", "0313-2/32660", 
                "0313-2/33400", "0313-2/33420", "0313-2/33440", "0313-2/33460", "0313-2/33480", "0313-2/33500", "0313-2/33520", "0313-2/33540", "0313-2/33560",
                "0313-2/35700", "0313-2/35720", "0313-2/35740", "0313-2/35760", "0313-2/35780", "0313-2/35800", "0313-2/35820", "0313-2/35840", "0313-2/35860",
                ]
                # Add clips from , 0531, 0601

    dirs = [base_dir + d for d in img_dirs]
    for d in dirs:
        if os.path.exists(d) == False:
            print("ERROR: ", d, " DOESN'T EXIST")
            assert False


    #if args.save_dir is not None and not ops.exists(args.save_dir):
    #    log.error('{:s} not exist and has been made'.format(args.save_dir))
    #    os.makedirs(args.save_dir)

    #print("USING SAVE DIRECTORY: ", args.save_dir)
    #cwd = os.getcwd()
    #print("CWD: ", cwd)

    for i, d in enumerate(dirs):
        print("Img seg ", str(i+1), " / ", str(len(dirs)))
        # test hnet model on a batch of image
        save_path = "output_frames/vid_seg" + str(i)
        if ops.exists(save_path) == False:
            os.makedirs(save_path)
        test_lanenet_batch(image_dir=d, 
                           weights_path="model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000",
                           save_dir=save_path, 
                           use_gpu=1, 
                           batch_size=1)
    print("All done)")
