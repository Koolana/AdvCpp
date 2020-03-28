project(home1)
cmake_minimum_required(VERSION 3.5)

add_library(process SHARED src/process.cpp)
target_include_directories(process PUBLIC include)

add_library(pipe SHARED src/pipe.cpp)
target_include_directories(pipe PUBLIC include)

add_executable(home1 main.cpp)
add_executable(homeProc1 mainProc.cpp)
target_link_libraries(process pipe)
target_link_libraries(home1 process)

