cmake_minimum_required(VERSION 3.20.0)

list(APPEND ZEPHYR_EXTRA_MODULES ${CMAKE_SOURCE_DIR}/modules/aw9364/aw9364)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(DeerMon)

target_sources(app PRIVATE src/main.c)
