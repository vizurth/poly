cmake_minimum_required(VERSION 3.10)
project(GraphTheoryLabs)

# установить стандарт C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# добавить флаги компилятора
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# исходные файлы для common/distributions
set(DISTRIBUTIONS_SOURCES
    common/distributions/weibull.cpp
)

# создать основной исполняемый файл для lab1
add_executable(lab1
    lab1/main.cpp
    lab1/src/ui.cpp
    ${DISTRIBUTIONS_SOURCES}
)

target_include_directories(lab1 PRIVATE 
    ${CMAKE_SOURCE_DIR}
)

# вывести информацию о сборке
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
