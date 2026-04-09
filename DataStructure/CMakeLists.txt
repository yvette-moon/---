cmake_minimum_required(VERSION 3.10)

project(opencv_test)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(/utf-8)
# OpenCV路径
include_directories(D:/EdgeLoad/opencv/build/include)

link_directories(D:/EdgeLoad/opencv/build/x64/vc16/lib)

add_executable(opencv_test
    main.cpp
    huffman.cpp
    
)

target_link_libraries(opencv_test opencv_world490)