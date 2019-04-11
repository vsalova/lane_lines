# About

TODO WRITE ABOUT

## Installation

### Requirements

* [Docker](https://www.docker.com/get-started)

### Instructions

1. Download the `Dockerfile` file from this repository

2. Navigate to the directory of the downloaded `Dockerfile` file

3. create a docker image from the Dockerfile:  
`docker build -t lane_lines .`  
note that `lane_lines` will be the name of the docker image create, so you can name it however you want.  
If you run `docker images` you should see an image with the tag `lane_lines`

4. create a docker container from this image:  
`docker create -it --name=ll lane_lines`  
The `--name=ll` creates a container with the name `ll`, which you can also change however you want.  
After running this command, if you type `docker ps -a`, you should see the created container.

5. Start and run the container  
`docker start ll`  
`docker exec ll /bin/bash`

6. Navigate to home: `cd /home`

7. Clone this repo: `git clone https://github.com/vsalova/lane_lines.git lanelines`

## Running

All different ways to run stuff

