cmake_minimum_required(VERSION 3.26)
project(mud_project C)
project(mud_client C)

set(CMAKE_C_STANDARD 11)
find_package(eclipse-paho-mqtt-c REQUIRED)

add_executable(mud_project mud_server.c)
target_link_libraries(mud_project eclipse-paho-mqtt-c::paho-mqtt3c)

add_executable(mud_client mud_client.c)