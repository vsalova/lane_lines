cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

project(sandbox)

set (OpenCV_DIR /root/installation/OpenCV-3.4.4/share/OpenCV)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

include_directories(include ${Boost_INCLUDE_DIRS})
link_libraries(${OpenCV_LIBS} ${Boost_LIBRARIES})

file(GLOB SOURCES "src/*.cpp")
add_executable(main ${SOURCES})
