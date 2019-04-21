GREEN='\e[0;32m'
RED='\e[0;31m'
BLUE='\e[34mBlue'
NC='\e[0m'
echo -e "${GREEN}ONE TIME BUILD SETUP${NC}"
echo -e "IF YOU WANT TO RECOMPILE THE SOURCE CODE GO INTO THE ${GREEN}build${NC} FOLDERS AND TYPE ${GREEN}make${NC}"
echo -e "Starting build in 2 seconds..."
sleep 2

orig_dir=$(pwd)

# Camera Calibration
echo # New line
echo -e "Building ${BLUE}camera_calibration${NC}..."
cd camera_calibration
mkdir build
cd build
cmake ..
make

# Lane Line Detection
echo # New line
echo -e "Building ${BLUE}lane_line_detection${NC}..."
cd ${orig_dir}
cd lane_line_detection
mkdir build
cd build
cmake ..
make

echo # New line
echo DONE

