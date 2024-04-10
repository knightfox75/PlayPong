#!/bin/bash

clear

if [ ! -d "linux_build_sim" ]; then
    mkdir linux_build_sim
fi

cd linux_build_sim || exit
cmake .. -DCMAKE_BUILD_TYPE=Release
make

cd ..

if [ "$1" != "-nopause" ]; then
    read -p "Press any key to continue..."
fi

"${PLAYDATE_SDK_PATH}/bin/PlaydateSimulator" PlayPong.pdx