cmake_minimum_required(VERSION 3.10)

project(CLI_Display LANGUAGES C)

set(SOURCES
	src/main.c
	src/math.c
)

add_executable(CLI_Display ${SOURCES})
target_include_directories(CLI_Display PRIVATE include)
