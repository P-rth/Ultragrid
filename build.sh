# This script is used for building the project as a whole without having to run each command separately.
mkdir -p build && cd ./build && clear && cmake .. && make && echo "--- build end ---" 
