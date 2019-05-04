Steps: Setting up environment
1: Connect to ATLAS with PUTTY
		- make sure to have the tunnel from localhost:5914 -> 5914

2: Run this to make a new vnc process
`TVNC_WM=mate-session /opt/TurboVNC/bin/vncserver :14`

3: Start TigerVNC (executable on desktop vncviewer64-1.9.0.exe)

4: Put localhost:5914 into TigerVNC

5: Enter password: My password is the normal one's first 8 characters

6: New terminal: Application -> System Tools -> MATE Terminal

7: Run the container using:
`docker exec -it -e DISPLAY=$DISPLAY lanenet /bin/bash`

8: Find the :14 line in here (make sure you are in your home directory first to find the .Xauthority file):
`xauth -f .Xauthority list`

9: Copy from the line that should look something like:

`atlas/unix:14  MIT-MAGIC-COOKIE-1  8a75a811a9cef219564946048a5c7f6b`

Copy this:

`:14  MIT-MAGIC-COOKIE-1  8a75a811a9cef219564946048a5c7f6b`

10: Paste

`xauth add <paste here>`

11: Run scripts! Should be done.


## Proper shutdown:
* List all vnc processes
`/opt/TurboVNC/bin/vncserver -list`

Or

`pgrep compiz`

* Stop vnc process

`/opt/TurboVNC/bin/vncserver -kill :14`

* Or 

`pkill compiz`

* Or 

`pkill -9 compiz`

Useful commands: 

`echo $DISPLAY`




Better to mount multiple things with   -v _:_ -v _:_ ... when making container
ie can mount multiple directories

