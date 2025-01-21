cmake_minimum_required(VERSION 3.10)

project(custom-lang)

# Compile flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Custom Variables

# Directories
set(CPP_DIR src/cpp/)
set(SCANNER_DIR src/cpp/scanner/)
set(COMPILER_DIR src/cpp/compiler/)
set(ERROR_DIR src/cpp/error)
set(SCOPE_DIR src/cpp/compiler/scope)
set(INCLUDE_DIR src/include)

# Setting source files
set(CPP_SOURCES ${CPP_DIR}/main.cpp
				${SCANNER_DIR}/Token.cpp
				${SCANNER_DIR}/Scanner.cpp
				${COMPILER_DIR}/expression/Value.cpp
				${COMPILER_DIR}/expression/Expr.cpp
				${COMPILER_DIR}/Parser.cpp
				${COMPILER_DIR}/ScopeStack.cpp
				${ERROR_DIR}/ScannerError.cpp
				${ERROR_DIR}/CompilerError.cpp
)

set(EXECUTABLE_NAME out)

include_directories(${INCLUDE_DIR})

add_executable(${EXECUTABLE_NAME} ${CPP_SOURCES})
