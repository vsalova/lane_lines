# LaneNet installation, training, evaluation instructions

## Initial steps to set up LaneNet and run pretrained model

* Step 0: Run this container on ATLAS:

	`nvidia-docker exec -it -e DISPLAY=$DISPLAY lanenet /bin/bash`

	This lanenet container is based on lanenet_x11_test image for visualizing output (run this if tkinter gives an error)

	`nvidia-docker exec -it -e DISPLAY=$DISPLAY lanenet_train /bin/bash`

	lanenet_train container to train model

* Step 1: Clone the repo

	`git clone https://github.com/MaybeShewill-CV/lanenet-lane-detection.git LaneNet`

* Step 2: Install requirements (`cd LaneNet` first)

	`pip install -r requirements.txt`

* Step 3: Container specific installation for `lanenet` container (we needed to install this specifically)

	`apt-get install python-tk python3-tk tk-dev`

* Step 4: Download pretrained lanenet model from [link](https://www.dropbox.com/sh/tnsf0lw6psszvy4/AAA81r53jpUI3wLsRW6TiPCya?dl=0)

* Step 5: Unzip the downloaded zipped model and place into `/model/tusimple_lanenet/`

* Step 6: Add folder to python3 path:

	`export PYTHONPATH="${PYTHONPATH}:/<path-to-lanenet-parentdir>/LaneNet/"`

	For us, the exported path was:

	`export PYTHONPATH="${PYTHONPATH}:/root/root_dit_atlas/home/cjcramer/lane_lines/LaneNet/"`


* Step 7: Run the test script:

	Note: make sure you use python3, so check what the `python --version` is, you may need to install python3.

	Note: be in the base directory of the repo when running this

	`python3 tools/test_lanenet.py --is_batch False --batch_size 1 --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path data/tusimple_test_image/0.jpg`

* Step 8: Run the test script on the some TuSimple images:

	`python3 tools/test_lanenet.py --is_batch True --batch_size 2 --save_dir data/tusimple_test_image/ret --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path data/tusimple_test_image/`

* Step 9: Run the test script on the some CuLane images: (these aren't included in the repo, but we added them, so they'll be in our repo)

	`python3 tools/test_lanenet.py --is_batch True --batch_size 2 --save_dir data/culane_test_image/ret --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path data/culane_test_image/`

* Step 10: Run the test script on the some random American road images

	`python3 tools/test_lanenet.py --is_batch True --batch_size 2 --save_dir data/random_american_roads_test_image/ret --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path data/random_american_roads_test_image/`

	I should perform poorly for anything other than the TuSimple images, since the downloaded model is only trained on TuSimple.



## Train on small test dataset:

See global_config.py file for configuring the training process.

Can do:
```
--net vgg
or
--net dense
```

When training

* Step 0: Download the vgg.npy weights (500 MB) from [link](https://mega.nz/#!YU1FWJrA!O1ywiCS2IiOlUCtCpI6HTJOMrneN-Qdv3ywQP5poecM)

* Step 1: Add the downloaded `vgg.npy` to the `data/` folder

* Step 2: Change all instances of `ROOT_DIR` in the `train.txt` and `text.txt` to your corresponding path. These files are in `data/training_data_example/`.

* Step 3: Train on GPU rather than CPU (skip to step 6 if you want to train on CPU):

	* Step 4: Make sure you have `tensorflow-gpu` installed, not `tensorflow`
		
		`pip install tensorflow-gpu`

		Note: If you run into issues with tensorflow, uninstall it, then only install tensorflow-gpu again

		Note: If you have issues with pickle, try downgrading numpy to the version required in `requirements.txt`

	* Step 5: Verify that tensorflow can see your gpu:
		```
		import tensorflow
		tensorflow.test.is_gpu_available()		# Should return True
```

* Step 6: Start training

	Note: before you start training, the checkpoint will be saved in the same folder as the pretrained tusimple folder in `model/tusimple_lanenet/`

	`python3 tools/train_lanenet.py --net vgg --dataset_dir data/training_data_example/`

* Optional Step 7: Continue training from a checkpoint

	`python3 tools/train_lanenet.py --net vgg --dataset_dir data/training_data_example/ --weights_path path/to/your/last/checkpoint`

Note: You can monitor the training using tensorboard:

	`python3 -m tensorboard.main`

Or

	`tensorboard --logdir=path/to/log-directory`

then connect in browser: `localhost:6006`

Although we did not do this.


## Train on CuLane

* Step 1: Reformat the CULane dataset to work with LaneNet

	* Run the `CULane_Convert_LaneNet.py` script, you'll need to change some paths in the python file to get the right locations for your dataset and the lanenet model

	* Follow the `CULane_remake_reformat_instructions.md` instructions. I recommend to first remake CULane to have better annotations first and then to convert it to be used for LaneNet.

* Step 2: Make sure you copied the `culane_dataset_lanenet` folder into the `<lanenet folder>/data/` folder.

* Step 3: In `tools/train_lanenet.py` change:

	* Line 134: `model_save_dir = 'model/culane_lanenet'`

	* Line 138: `model_name = 'culane_lanenet_{:s}_{:s}.ckpt'.format......`

	* Line 142: `tboard_save_path = 'tboard/culane_lanenet/{:s}'.format(net_flag)`

	Change to L1 normalization: in `lanenet_model\lanenet_discriminative_loss.py`:

	* For Lines 57, 84, 92, change `tf.norm(....., axis=1)` to include a `ord=1` option: `tf.norm(....., axis=1, ord=1)`	

* Step 4: Modify hyperparameters in `config\global_config.py`:

	* Change the training learning rate from `0.0005` to `0.0004`

	* Change the batch size from 8 to 14

* Step 5: Start training from TuSimple model (recommended)

	`python3 tools/train_lanenet.py --net vgg --dataset_dir data/culane_dataset_lanenet/ --weights_path model/culane_lanenet/checkpoint`

* Step 6: Start training from scratch

	`python3 tools/train_lanenet.py --net vgg --dataset_dir data/culane_dataset_lanenet/`

* Step 7: Start training from checkpoint

	`python3 tools/train_lanenet.py --net vgg --dataset_dir data/culane_dataset_lanenet/ --weights_path model/tusimple_lanenet_orig/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000`

	After 3600 epochs:

	`python3 tools/train_lanenet.py --net vgg --dataset_dir data/culane_dataset_lanenet/ --weights_path model/culane_lanenet/culane_lanenet_vgg_2019-05-06-23-28-26.ckpt-2000`


## Infer on Video 


Make sure to be in the base directory of LaneNet

Have the `output_frames` directory created in the base directory of LaneNet

Testing running batch

	`python3 tools/test_lanenet.py --is_batch True --batch_size 1 --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path data/tusimple_test_image/ --use_gpu 0 --save_dir output_frames/`


Test on TuSimple folder

	`python3 tools/test_lanenet.py --is_batch True --batch_size 1 --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path /root/root_dit_atlas/teams/team-1/Datasets/TuSimple/train_set/clips/0313-2/58980 --use_gpu 0 --save_dir output_frames/vid1`

Another teston a TuSimple folder

	`python3 tools/test_lanenet.py --is_batch True --batch_size 1 --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path /root/root_dit_atlas/teams/team-1/Datasets/TuSimple/train_set/clips/0313-2/13380 --use_gpu 0 --save_dir output_frames/vid2`

	...

Combine the images into video

See the `CULane_remake_reformat_instructions.md` file in `Scripts/` folder.


## Infer on Video with postprocessing

	Make sure you are in the root directory of LaneNet

	You'll want to go into the python file to change some stuff probably.

	`python3 tools/test_lanenet_postprocess.py`
