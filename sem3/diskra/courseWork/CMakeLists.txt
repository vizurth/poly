cmake_minimum_required(VERSION 3.10)
project(diskra)

# Настройка стандартов и флагов компиляции
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Дополнительные флаги
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

# Пути
include_directories(include)

# Источники
set(SOURCES
    src/main.cpp
    src/SmallArithmetic.cpp
    src/BigArithmeticCalc.cpp
    src/UI.cpp
)

# Создание исполняемого файла
add_executable(diskra ${SOURCES})
