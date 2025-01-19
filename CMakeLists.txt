cmake_minimum_required(VERSION 3.10)

project(custom-lang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_SOURCE_DIR ./src)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(out src/cpp/scanner/Token.cpp src/cpp/main.cpp src/cpp/error/ScannerError.cpp src/cpp/scanner/Scanner.cpp src/cpp/compiler/expression/Value.cpp) 
