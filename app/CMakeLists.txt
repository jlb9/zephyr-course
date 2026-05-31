cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND DTS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(DTC_OVERLAY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/app.overlay)

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_sources(app PRIVATE
    src/main.cpp
    src/sensor_shell.c
)
add_subdirectory(driver/our_driver)
