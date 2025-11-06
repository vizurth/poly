cmake_minimum_required(VERSION 3.16)
project(DynamicWidgetCollection VERSION 1.0 LANGUAGES CXX)

# --- Настройки CMake ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Пути к Qt ---
# Если Qt не находится сам, укажи путь вручную:
# set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.9.2/lib/cmake")

# --- Подключаем нужные модули Qt ---
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- Автоматическая обработка moc/uic/rcc ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --- Источники и заголовки ---
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/headers)

set(SOURCE_FILES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/mainwindow.cpp
)

set(HEADER_FILES
    ${HEADERS_DIR}/mainwindow.h
    ${HEADERS_DIR}/rectangle.h
    ${HEADERS_DIR}/triangle.h
    ${HEADERS_DIR}/ellipse.h
)

# --- Создание исполняемого файла ---
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# --- Линковка с Qt ---
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# --- Для macOS: собираем .app bundle ---
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
)

# --- Пути к заголовкам ---
target_include_directories(${PROJECT_NAME} PRIVATE ${HEADERS_DIR})
