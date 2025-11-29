cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(SDNF_Builder)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исполняемый файл
add_executable(sdnf
    src/main.cpp
)
