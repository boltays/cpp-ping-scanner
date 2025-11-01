cmake_minimum_required(VERSION 3.10)
project(PingScanner VERSION 0.1.0)

# C standard 
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#executable
add_executable(ping_scanner 
	src/main.cpp
	src/icmp_protocol.c 
)

target_include_directories(ping_scanner PRIVATE include)

