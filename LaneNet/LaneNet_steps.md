Initial steps to set up LaneNet

1: Clone the repo
`git clone https://github.com/MaybeShewill-CV/lanenet-lane-detection.git LaneNet`

2: Install requirements (`cd LaneNet`)
`pip install -r requirements.txt`

3: Container specific installation (we needed to install this specifically)

`apt-get install python-tk python3-tk tk-dev`

4: Download pretrained lanenet model: https://www.dropbox.com/sh/tnsf0lw6psszvy4/AAA81r53jpUI3wLsRW6TiPCya?dl=0

5: Unzip the zip file in in `/model/tusimple_lanenet/`

6: Add folder to python path
`export PYTHONPATH="${PYTHONPATH}:/<path-to-lanenet-parentdir>/LaneNet/lanenet_model"`

7: Run the test script

Note: make sure you use python3, so check what the `python --version` is, you may need to install python3 or use `python3 instead`.

`python tools/test_lanenet.py --is_batch False --batch_size 1 --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path data/tusimple_test_image/0.jpg`

8: Run the test script on the some TuSimple images

`python tools/test_lanenet.py --is_batch True --batch_size 2 --save_dir data/tusimple_test_image/ret --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path data/tusimple_test_image/`

9: Run the test script on the some CuLane images

`python tools/test_lanenet.py --is_batch True --batch_size 2 --save_dir data/culane_test_image/ret --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path data/culane_test_image/`

10: Run the test script on the some some American road images

`python tools/test_lanenet.py --is_batch True --batch_size 2 --save_dir data/random_american_roads_test_image/ret --weights_path model/tusimple_lanenet/tusimple_lanenet_vgg_2018-10-19-13-33-56.ckpt-200000 --image_path data/random_american_roads_test_image/`



Train on CuLane


See global_config.py file

Can do:
--net vgg
--net dense

Start training example:

First download the vgg.npy weights (500 MB):

https://mega.nz/#!YU1FWJrA!O1ywiCS2IiOlUCtCpI6HTJOMrneN-Qdv3ywQP5poecM

`python tools/train_lanenet.py --net vgg --dataset_dir data/training_data_example/`

Continue training from a checkpoint:

`python tools/train_lanenet.py --net vgg --dataset_dir data/training_data_example/ --weights_path path/to/your/last/checkpoint`

Monitor training using tensorboard tools?

Try:

`python -m tensorboard.main`

Or

`tensorboard --logdir=path/to/log-directory`
then connect in browser: `localhost:6006`

