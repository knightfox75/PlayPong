@echo off
cls

if not exist win_build_sim mkdir win_build_sim
cd win_build_sim
cmake .. -DWIN_MINGW_COMPILER=ON -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles"
nmake

cd..

if  not "%1"=="-nopause" pause

%PLAYDATE_SDK_PATH%\bin\PlaydateSimulator.exe PlayPong.pdx

exit