cmake_minimum_required(VERSION 3.19)
project(DiskraCourseWork)

set(CMAKE_CXX_STANDARD 17)

add_executable(DiskraCourseWork main.cpp Base64.cpp Base64.h HashFunction.cpp
        HashFunction.h Keccak.h Keccak.cpp stdafx.cpp stdafx.h Hex.h ParserCommon.h
        RAII_Wrapper.h Endian.h Rotation.h)