cmake_minimum_required(VERSION 3.10)
project(PingScanner VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#executable
add_executable(ping_scanner src/main.cpp)

