# About

Lane line detection project for EEC193a/b

## Installation

### Requirements

* [Docker](https://www.docker.com/get-started)

### Instructions

1. Download the `Dockerfile` file from this repository.

2. Navigate to the directory of the downloaded `Dockerfile` file.

3. Create a docker image from the Dockerfile: `docker build -t lane_lines .` note that `lane_lines` will be the name of the docker image create, so you can name it however you want.
 
4. If you run `docker images` you should see an image with the tag `lane_lines`. Comment the last line in the Dockerfile if you're gonna mount the container to a specific folder. 

4. Create a docker container from this image and mount it to your local lanelines repository: `docker run -it --name=<container name> --privileged -v <path to a local repo>:/root/<root folder name> <image name> /bin/bash`. After running this command, if you type `docker ps -a`, you should see the created container. To run the docker container, make sure to do: `xhost + <IP>`, then start the docker container, and then to enter it, type: `docker exec -it -e DISPLAY=<IP>:0.0 ll /bin/bash`

5. Start and run just the container:  
`docker start <container name>`  
`docker attach <container name>`

6. Navigate to home: `cd /home`

7. Clone this repo: `git clone https://github.com/vsalova/lane_lines.git lanelines`

## Running

###To run ICNet:

1. A container should be created using this command: `nvidia-docker create -it --name=icnet --privileged -e NVIDIA_DRIVER_CAPABILITIES=utility,compute -e NVIDIA_VISIBLE_DEVICES=all -v /:/root/atlast_root -p 9007:9007 vidia/cuda:10.0-cudnn7-devel-ubuntu16.04 /bin/bash`. This function uses image nvidia/cuda:10.0-cudnn7-devel-ubuntu16.04 and adds port forwarding.
2. SSH into the server: `ssh  -L 9007:localhost:9007 username@server`
3. Run `jupyter notebook --no-browser --ip=0.0.0.0 --allow-root --port=9007`
3. In browser, access the jupyter notebook using `localhost:9007`
4. Docker container should be entered using:  
`nvidia-docker start <container name>`  
`nvidia-docker attach <container name>`
5. Python 3 and pip3 will have to be installed within the container
6. To train ICNet, navigate to ICnet folder and edit the utils.py file
7. The run: `python3 train.py --update-mean-var --train-beta-gamma --random-scale --random-mirror --dataset=culane --filter-scale 1`
8. To evaluate: `python3 evaluate.py --dataset=culane --filter-scale=1 --model=others`


# Orginal Lane Line Detection Instructions

The original lane line lab from January 2019 is located in `lab1\lab1_merged`

## Installation Requirement

* cmake [download page](https://cmake.org/download/)


## Build

Run the `lab1\lab1_merged\build.sh` script by typing:  
`./build.sh`

## Run

TODO get input files syntax for each executable

Run any of the executables

* `camera_calibration/build/calibrate_camera INPUTS HERE`
* `camera_calibration/build/intrinsics_tester INPUTS HERE`

* `lane_line_detection/build/images_main INPUTS HERE`
* `lane_line_detection/build/perspective_transform_tester INPUTS HERE`
* `lane_line_detection/build/thresholds_tester INPUTS HERE`
* `lane_line_detection/build/video_main INPUTS HERE`
* `lane_line_detection/build/window_search_tester INPUTS HERE`

You can run the program after building by executing `./make_video.sh` 
