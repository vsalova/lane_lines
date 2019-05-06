# Remake CULane to have better annotations:

* Step 1. Make sure your port bound in Putty first while connecting to your server

* Step 2. Make Container: (where 9001 is whatever port you want)

	Note: you will need a docker image that has opencv, python3, and jupyter notebook

	`docker run -it --name=culane --privileged -p 9001:9001 -v /:/root/atlas_root lane_lines /bin/bash`

* Step 3. Start and run container
 	
 	`docker start culane`

 	`docker attach culane`

* Step 4. Start jupyter notebook in container
	`jupyter notebook --no-browser --ip=0.0.0.0 --allow-root --port=9001`
	
	Note: copy token if starting first time

* Step 5. Enter localhost:9001 in web browser to access

* Optional Step 6. Convert jupyer notebook to python script:

	`jupyter nbconvert --to script CULane_Remake.ipynb`

* Step 7. Run the python script

	`python3 CULane_Remake.py` to see the output for just the first image

	`python3 CULane_Remake.py --no-vis` to run through the whole dataset

* Note for ATLAS server: I made an image after installed everything called `cv2_jupyter`

* Use `docker commit` to convert docker container to image


# Convert CULane to be used with LaneNet model

* Step 0: Make this script:

    `jupyter nbconvert --to script CULane_Convert_LaneNet.ipynb`

* Step 1: Run the script:
    
    `python3 CULane_Convert_LaneNet.py`

    Or

    `python3 CULane_Convert_LaneNet.py --no-vis`

* Step 2: Copy the resulting folder to your LaneNet/data folder

* Note this creates a 90 10 train validation split of the around 100k annotated images in CULane
