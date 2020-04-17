set(SRC_SERVER
    "serverMain.cpp"
)

add_executable(server ${SRC_SERVER})
target_link_libraries(server ${NAME_LIB})

