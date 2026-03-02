cmake_minimum_required(VERSION 3.16)

project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(test
    src/main.cpp
    src/prng.cpp
)