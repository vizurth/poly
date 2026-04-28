cmake_minimum_required(VERSION 3.10)
project(GraphTheoryLabs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(COMMON_SOURCES
    common/distribution/weibull.cpp
)

add_executable(tgraph
    main.cpp
    lab1/src/ui.cpp
    lab2/src/ui.cpp
    lab3/src/ui.cpp
    lab4/src/ui.cpp
    lab4/src/kruskal.cpp
    lab4/src/kirchhoff.cpp
    lab4/src/prufer.cpp
    lab4/src/mis.cpp
    ${COMMON_SOURCES}
)

target_include_directories(tgraph PRIVATE
    ${CMAKE_SOURCE_DIR}
)

message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
