cmake_minimum_required(VERSION 3.10)

project(CLI_Display LANGUAGES C)

set(SOURCES
	src/main.c
	src/Render.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_subdirectory(libs/SjenaDesk_Engine)
target_link_libraries(${PROJECT_NAME} PRIVATE SjenaDesk_Engine)
