cmake_minimum_required(VERSION 3.26)
project(mud_project C)
project(mud_client C)

set(CMAKE_C_STANDARD 11)

add_executable(mud_project mud_server.c)
add_executable(mud_client mud_client.c)