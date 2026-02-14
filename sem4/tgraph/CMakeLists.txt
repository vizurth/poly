cmake_minimum_required(VERSION 3.10)
project(GraphTheoryLabs)

# установить стандарт C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# добавить флаги компилятора
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# исходные файлы для common/distributions
set(DISTRIBUTIONS_SOURCES
    common/distributions/distribution.cpp
    common/distributions/normal.cpp
)

# исходные файлы для common/graph
set(GRAPH_SOURCES
    common/graph/graph.cpp
    common/graph/dag_generator.cpp
    common/graph/dijkstra.cpp
    common/graph/bellman_ford.cpp
)

# исходные файлы для lab1
set(LAB1_SOURCES
    lab1/src/main.cpp
    lab1/src/ui.cpp
    lab1/src/shimbel.cpp
    lab1/src/route_counter.cpp
)

# исходные файлы для lab3
set(LAB3_SOURCES
    lab3/src/main.cpp
    lab3/src/ui.cpp
    lab3/src/capacity_generator.cpp
    lab3/src/ford_fulkerson.cpp
    lab3/src/min_cost_flow.cpp
)

# исходные файлы для lab5
set(LAB5_SOURCES
    lab5/src/main.cpp
    lab5/src/ui.cpp
    lab5/src/euler.cpp
    lab5/src/hamilton.cpp
    lab5/src/tsp.cpp
)

# создать исполняемый файл для lab1
add_executable(lab1 
    ${LAB1_SOURCES}
    ${GRAPH_SOURCES}
    ${DISTRIBUTIONS_SOURCES}
)

# добавить директории с заголовочными файлами
target_include_directories(lab1 PRIVATE 
    ${CMAKE_SOURCE_DIR}
)

# создать исполняемый файл для lab3
add_executable(lab3
    ${LAB3_SOURCES}
    ${GRAPH_SOURCES}
    ${DISTRIBUTIONS_SOURCES}
)

# добавить директории с заголовочными файлами
target_include_directories(lab3 PRIVATE 
    ${CMAKE_SOURCE_DIR}
)

# создать исполняемый файл для lab5
add_executable(lab5
    ${LAB5_SOURCES}
    ${GRAPH_SOURCES}
    ${DISTRIBUTIONS_SOURCES}
)

# добавить директории с заголовочными файлами
target_include_directories(lab5 PRIVATE 
    ${CMAKE_SOURCE_DIR}
)

# вывести информацию о сборке
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
