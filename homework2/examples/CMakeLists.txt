cmake_minimum_required(VERSION 3.5)

project(example)

set(SRC_EXAMPLE
    "${PROJECT_SOURCE_DIR}/main.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_EXAMPLE})
target_link_libraries(${PROJECT_NAME} ${NAME_LIB})
