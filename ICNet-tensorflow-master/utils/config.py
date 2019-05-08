import numpy as np
import os

class Config(object):
    # Setting dataset directory
    CITYSCAPES_DATA_DIR = './data/cityscapes_dataset/cityscape/'
    ADE20K_DATA_DIR = './data/ADEChallengeData2016/'
    CULANE_DATA_DIR = '/root/atlast_root/teams/team-1/Datasets/CULane_Dataset_RGB/'

    ADE20K_eval_list = os.path.join('./data/list/ade20k_val_list.txt')
    CITYSCAPES_eval_list = os.path.join('./data/list/cityscapes_val_list.txt')
    CULANE_eval_list = os.path.join('./data/list/val_gt.txt')


    ADE20K_train_list = os.path.join('./data/list/ade20k_train_list.txt')
    CITYSCAPES_train_list = os.path.join('./data/list/cityscapes_train_list.txt')
    CULANE_train_list = os.path.join('./data/list/train_gt.txt')

    #used to normalize -- will have to adjust this or look up the mean
    IMG_MEAN = np.array((94.09, 93.74, 92.29), dtype=np.float32)

    ADE20k_param = {'name': 'ade20k',
                'num_classes': 150, # predict: [0~149] corresponding to label [1~150], ignore class 0 (background)
                'ignore_label': 0,
                'eval_size': [480, 480],
                'eval_steps': 2000,
                'eval_list': ADE20K_eval_list,
                'train_list': ADE20K_train_list,
                'data_dir': ADE20K_DATA_DIR}

    cityscapes_param = {'name': 'cityscapes',
                    'num_classes': 19,
                    'ignore_label': 255,
                    'eval_size': [1025, 2049],
                    'eval_steps': 500,
                    'eval_list': CITYSCAPES_eval_list,
                    'train_list': CITYSCAPES_train_list,
                    'data_dir': CITYSCAPES_DATA_DIR}

    model_paths = {'train': './model/cityscapes/icnet_cityscapes_train_30k.npy',
              'trainval': './model/cityscapes/icnet_cityscapes_trainval_90k.npy',
              'train_bn': './model/cityscapes/icnet_cityscapes_train_30k_bnnomerge.npy',
              'trainval_bn': './model/cityscapes/icnet_cityscapes_trainval_90k_bnnomerge.npy',
              #'others': './model/ade20k/model.ckpt-27150'}
              'others': './snapshots/model.ckpt-60000'}

    ## If you want to train on your own dataset, try to set these parameters.
    CULane_param = {'name': 'culane',
                'num_classes': 5,
                'ignore_label': 100,
                'eval_size': [480, 480],
                'eval_steps': 2000,
                'eval_list': CULANE_eval_list,
                'train_list': CULANE_train_list,
                'data_dir': CULANE_DATA_DIR}

    ## You can modify following lines to train different training configurations.
    INFER_SIZE = [1024, 2048, 3]
    TRAINING_SIZE = [720, 720]
    TRAINING_STEPS = 60001

    N_WORKERS = 8
    BATCH_SIZE = 16
    LEARNING_RATE = 1e-4
    MOMENTUM = 0.9
    POWER = 0.9
    RANDOM_SEED = 1234
    WEIGHT_DECAY = 0.0001
    SNAPSHOT_DIR = './snapshots/'
    SAVE_NUM_IMAGES = 4
    SAVE_PRED_EVERY = 500

    # Loss Function = LAMBDA1 * sub4_loss + LAMBDA2 * sub24_loss + LAMBDA3 * sub124_loss
    LAMBDA1 = 0.16
    LAMBDA2 = 0.4
    LAMBDA3 = 1.0

    def __init__(self, dataset, is_training=False, filter_scale=1, random_scale=False, random_mirror=False):
        print('Setup configurations...')

        if dataset == 'ade20k':
            self.param = self.ADE20k_param
        elif dataset == 'cityscapes':
            self.param = self.cityscapes_param
        elif dataset == 'culane':
            self.param = self.CULane_param

        self.dataset = dataset
        self.random_scale = random_scale
        self.random_mirror = random_mirror
        self.is_training = is_training
        self.filter_scale = filter_scale

    def display(self):
        """Display Configuration values."""
        print("\nConfigurations:")
        for a in dir(self):
            if not a.startswith("__") and not callable(getattr(self, a)) and not isinstance(getattr(self, a), dict):
                print("{:30} {}".format(a, getattr(self, a)))

            if a == ("param"):
                print(a)
                for k, v in getattr(self, a).items():
                    print("   {:27} {}".format(k, v))

        print("\n")
