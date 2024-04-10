cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_STANDARD 11)


# Define the project name
set(PROJECT_NAME PlayPong)


# Define options for MinGW compiler (Windows, Simulator)
if (WIN32)
	if (NOT EXISTS $ENV{MINGW64_PATH})
		message(FATAL_ERROR "The environment variable MINGW64_PATH is not correctly defined.")
		return()
	else()
		option(WIN_MINGW_COMPILER "Use Windows MinGW compiler" OFF)
		if(WIN_MINGW_COMPILER)
			find_program(WIN_MINGW_GCC_FOUND x86_64-w64-mingw32-gcc.exe)
			if(NOT WIN_MINGW_GCC_FOUND)
				message(FATAL_ERROR "x86_64-w64-mingw32-gcc.exe not found in the system PATH")
				return()
			else()
				set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc.exe)
				set(CMAKE_ASM_COMPILER x86_64-w64-mingw32-gcc.exe)
			endif()
		endif()
	endif()
endif()


# Define options for Playdate SDK
set(ENVSDK $ENV{PLAYDATE_SDK_PATH})

if (NOT ${ENVSDK} STREQUAL "")
    # Convert path from Windows
    file(TO_CMAKE_PATH ${ENVSDK} SDK)
else()
    execute_process(
            COMMAND bash -c "egrep '^\\s*SDKRoot' $HOME/.Playdate/config"
            COMMAND head -n 1
            COMMAND cut -c9-
            OUTPUT_VARIABLE SDK
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

if (NOT EXISTS ${SDK})
    message(FATAL_ERROR "The environment variable PLAYDATE_SDK_PATH is not correctly defined.")
    return()
endif()


set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)


# Game Name Customization
# Setting up constants for the game name and device name
set(PLAYDATE_GAME_NAME ${PROJECT_NAME})
set(PLAYDATE_GAME_DEVICE ${PROJECT_NAME}_DEVICE)

project(${PLAYDATE_GAME_NAME} C ASM)


# Includes all .c and .h files in Src and its subfolders.
file(GLOB_RECURSE SRC_FILES "Src/*.c")
file(GLOB_RECURSE INC_FILES "Src/*.h")


if (TOOLCHAIN STREQUAL "armgcc")
    add_executable(${PLAYDATE_GAME_DEVICE} ${SDK}/C_API/buildsupport/setup.c ${SRC_FILES})
else()
    add_library(${PLAYDATE_GAME_NAME} SHARED ${SRC_FILES} ${INC_FILES})
endif()

include(${SDK}/C_API/buildsupport/playdate_game.cmake)