cmake_minimum_required(VERSION 4.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(program)
set(INCLUDE_DIR "./include")
set(LIBRARY_DIR "./lib")
set(EXECUTABLE_OUTPUT_PATH "./bin")
set(LIBRARY_OUTPUT_PATH "./lib")

file(
    GLOB_RECURSE SOURCE_LIST
    RELATIVE ${CMAKE_SOURCE_DIR}
    "./src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE "glfw3")
