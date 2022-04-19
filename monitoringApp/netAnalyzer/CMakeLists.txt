cmake_minimum_required(VERSION 3.13)  # CMake version check
project(gameUsageAnalizaer)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES analyzer.cpp)


# Add executable target with source files listed in SOURCE_FILES variable
add_executable(gameUsageAnalizaer ${SOURCE_FILES})
target_link_libraries(
      gameUsageAnalizaer
      Iphlpapi 
)

