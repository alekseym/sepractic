# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)

project(cloudDetector)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(cloudDetector cloud.cpp)
target_link_libraries(cloudDetector ${DEPENDENCIES} ${OpenCV_LIBS})



#if(BUILD_GRAPHICAL_EXAMPLES)
#    add_executable(depthcv rs-pointcloud.cpp ../example.hpp)
#    set_property(TARGET depthcv PROPERTY CXX_STANDARD 11)
#    target_link_libraries(depthcv ${DEPENDENCIES} ${OpenCV_LIBS})
#    include_directories(../)
#    set_target_properties (depthcv PROPERTIES FOLDER Examples)
#    install(TARGETS depthcv RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
#endif()
