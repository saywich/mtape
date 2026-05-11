cmake_minimum_required(VERSION 3.22)

project(
    mtape
    DESCRIPTION "Magnet tape for yadro internship"
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/include)

add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} PRIVATE mtapelib)

enable_testing()
add_subdirectory(tests)
