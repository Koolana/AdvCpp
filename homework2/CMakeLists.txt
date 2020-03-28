cmake_minimum_required(VERSION 3.5)
project(log)

add_compile_options(-std=c++14)

set(NAME_LIB ${PROJECT_NAME})

set(SRC_LOG
    "${PROJECT_SOURCE_DIR}/src/baselogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/stdoutlogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/stderrlogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/filelogger.cpp"
    "${PROJECT_SOURCE_DIR}/src/logger.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SRC_LOG})

FILE(GLOB_RECURSE LibFiles "${PROJECT_SOURCE_DIR}/include/*")
add_custom_target(headers SOURCES ${LibFiles})

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_subdirectory(examples)
