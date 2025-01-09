cmake_minimum_required(VERSION 3.10)

project(custom-lang)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCE_DIR ./src)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(out src/cpp/Token.cpp src/cpp/main.cpp)
