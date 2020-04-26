cmake_minimum_required(VERSION 3.5)
project(tcp)

add_compile_options(-std=c++14)

set(NAME_LIB ${PROJECT_NAME})

set(SRC_LOG
    "src/server.cpp"
    "src/connection.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SRC_LOG})

FILE(GLOB_RECURSE LibFiles "include/*")
add_custom_target(headers SOURCES ${LibFiles})

target_include_directories(${PROJECT_NAME} PUBLIC "include")

add_subdirectory(examples)
