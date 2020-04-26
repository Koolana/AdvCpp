set(SRC_SERVER
    "serverMain.cpp"
)

add_executable(server ${SRC_SERVER})
target_link_libraries(server ${NAME_LIB})

set(SRC_CLIENT
    "clientMain.cpp"
)

add_executable(client ${SRC_CLIENT})
target_link_libraries(client ${NAME_LIB})
