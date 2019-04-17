FROM spmallick/opencv-docker:opencv
RUN apt-get update -y
RUN apt-get install libboost-all-dev -y
RUN apt-get install ffmpeg -y
RUN apt-get install python-numpy -y
RUN cd ~
RUN git clone https://github.com/lava/matplotlib-cpp.git
RUN cp matplotlib-cpp/matplotlibcpp.h /usr/include/
RUN rm -rf matplotlib-cpp
