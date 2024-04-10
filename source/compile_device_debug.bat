@echo off
cls

if not exist win_build_dev mkdir win_build_dev
cd win_build_dev
cmake .. -DCMAKE_BUILD_TYPE=Debug -G "NMake Makefiles" --toolchain=%PLAYDATE_SDK_PATH%/C_API/buildsupport/arm.cmake
nmake

cd..

if  not "%1"=="-nopause" pause

exit