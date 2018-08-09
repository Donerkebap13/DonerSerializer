#!/bin/bash

ROOT=`pwd`
PROJECT_PATH=${ROOT}/project_clang
EXTRA_CMAKE_PARAMETERS=$1

if [ ! -d ${PROJECT_PATH} ]; then
    mkdir -p ${PROJECT_PATH}
fi

cd ${PROJECT_PATH}

export CC=C:/msys64/mingw32/bin/clang.exe
export CXX=C:/msys64/mingw32/bin/clang++.exe

cmake "../DonerSerializer" -G"MSYS Makefiles" -DDONER_ENABLE_TESTS=1 ${EXTRA_CMAKE_PARAMETERS}

make -j8
