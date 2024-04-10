#!/bin/bash
clear

if [ ! -d "linux_build_dev" ]; then
    mkdir linux_build_dev
fi

cd linux_build_dev || exit
cmake .. -DCMAKE_BUILD_TYPE=Debug --toolchain=${PLAYDATE_SDK_PATH}/C_API/buildsupport/arm.cmake
make

cd ..

if [ "$1" != "-nopause" ]; then
    read -p "Press any key to continue..."
fi