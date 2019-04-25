# Compile with -O3

# Running make_video_benchmark

# Running on GPU, numbers are FPS

### Calvin's laptop

* Total frames = `628`

#### CPU: i7-7500U CPU @ 2.70GHz (2 core)
* total: 353.116 seconds,                                 FPS: 1.77845 /s
* total_time_undistort-- total: 93.0972 seconds,  		  FPS: 6.74564 /s
* total_time_warp-- total: 33.4666 seconds,       		  FPS: 18.765 /s
* total_time_thresh-- total: 151.959 seconds,     		  FPS: 4.1327 /s
* total_time_convert_color-- total: 2.02782 seconds,      FPS: 309.692 /s
* total_time_find_peaks-- total: 0.882106 seconds,        FPS: 711.933 /s
* total_time_window_search-- total: 32.115 seconds,       FPS: 19.5547 /s
* total_time_waypoints-- total: 0.067467 seconds,         FPS: 9308.25 /s
* total_time_distort_back-- total: 30.6202 seconds,       FPS: 20.5093 /s
* total_time_add_weighted-- total: 7.68296 seconds,       FPS: 81.7394 /s

- FPS without threshold_time: 3.12194 /s

### Kronos
#### CPU: Xeon W-2102 CPU @ 2.90GHz (4 core)
* Total time-- total: 96.4737 seconds,    FPS: 6.50955 /s
* total_time_undistort-- total: 21.0902 seconds,  FPS: 29.7768 /s
* total_time_warp-- total: 7.76467 seconds,       FPS: 80.8792 /s
* total_time_thresh-- total: 50.1021 seconds,     FPS: 12.5344 /s
* total_time_convert_color-- total: 1.35677 seconds,      FPS: 462.865 /s
* total_time_find_peaks-- total: 0.297707 seconds,        FPS: 2109.46 /s
* total_time_window_search-- total: 5.48399 seconds,      FPS: 114.515 /s
* total_time_waypoints-- total: 0.029802 seconds,         FPS: 21072.4 /s
* total_time_distort_back-- total: 9.15255 seconds,       FPS: 68.6148 /s
* total_time_add_weighted-- total: 0.658458 seconds,      FPS: 953.743 /s

- FPS without threshold_time: 13.54277 /s

### Jetson Tx2
<<<<<<< HEAD
#### CPU: Arm64 Cortex-A9 1.2GHz (2 core)
=======
#### CPU: 
* Total time-- total: 252.727 seconds, 	FPS: 2.48489 /s
* total_time_undistort-- total: 69.1702 seconds, 	FPS: 9.07906 /s
* total_time_warp-- total: 30.4818 seconds, 	FPS: 20.6025 /s
* total_time_thresh-- total: 106.363 seconds, 	FPS: 5.90432 /s
* total_time_convert_color-- total: 0.532603 seconds, 	FPS: 1179.11 /s
* total_time_find_peaks-- total: 1.96466 seconds, 	FPS: 319.648 /s
* total_time_window_search-- total: 13.2352 seconds, 	FPS: 47.4493 /s
* total_time_waypoints-- total: 0.050319 seconds, 	FPS: 12480.4 /s
* total_time_distort_back-- total: 27.3426 seconds, 	FPS: 22.9678 /s
* total_time_add_weighted-- total: 3.04364 seconds, 	FPS: 206.332 /s

- FPS without threshold_time: 4.29067 /s
