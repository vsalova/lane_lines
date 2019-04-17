cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

project(camera_calibration)

set (OpenCV_DIR /root/installation/OpenCV-3.4.4/share/OpenCV)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

include_directories(include ${Boost_INCLUDE_DIRS})
link_libraries(${OpenCV_LIBS} ${Boost_LIBRARIES})

add_executable(calibrate_camera "src/calibrate_camera.cpp" "src/cv_helper.cpp")

add_executable(intrinsics_tester "src/intrinsics_tester.cpp" "src/cv_helper.cpp")
