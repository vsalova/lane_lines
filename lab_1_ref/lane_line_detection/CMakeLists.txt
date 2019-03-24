cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

project(lane_line_detection)

set (OpenCV_DIR /root/installation/OpenCV-3.4.4/share/OpenCV)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(PythonLibs 2.7 REQUIRED)

include_directories(include ${Boost_INCLUDE_DIRS})
link_libraries(${OpenCV_LIBS} ${Boost_LIBRARIES})

#file(GLOB SOURCES "src/*.cpp")
add_executable(perspective_transform_tester "src/perspective_transform_tester.cpp" "src/cv_helper.cpp")

add_executable(thresholds_tester "src/thresholds_tester.cpp" "src/thresholds.cpp" "src/cv_helper.cpp")

add_executable(window_search_tester "src/window_search_tester.cpp" "src/window_search.cpp" "src/thresholds.cpp" "src/signal_proc.cpp" "src/lane_line.cpp" "src/cv_helper.cpp")
target_include_directories(window_search_tester PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(window_search_tester ${PYTHON_LIBRARIES})

add_executable(images_main "src/images_main.cpp" "src/window_search.cpp" "src/thresholds.cpp" "src/signal_proc.cpp" "src/lane_line.cpp" "src/cv_helper.cpp")

add_executable(video_main "src/video_main.cpp" "src/window_search.cpp" "src/thresholds.cpp" "src/signal_proc.cpp" "src/lane_line.cpp" "src/cv_helper.cpp")
