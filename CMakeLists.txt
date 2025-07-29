cmake_minimum_required(VERSION 3.20)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPERIMENTAL_CXX_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(program)
set(INCLUDE_DIR "./include" "./src")
set(LIBRARY_DIR "./lib")
set(EXECUTABLE_OUTPUT_PATH "./bin")
set(LIBRARY_OUTPUT_PATH "./lib")

file(
    GLOB_RECURSE SOURCE_LIST
    RELATIVE ${CMAKE_SOURCE_DIR}
    "./src/*.cpp" "./src/*.c")

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE "glfw3")
