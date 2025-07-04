cmake_minimum_required(VERSION 3.15)
project(TextEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(caesar SHARED
        src/mylib.cpp
)

target_compile_definitions(caesar
        PRIVATE BUILD_DLL
)

set_target_properties(caesar PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(caesar
        PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_executable(${PROJECT_NAME}
        main.cpp
        func/editing_text.cpp
        func/functionality.cpp
        src/line.cpp
        func/pages.cpp
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE caesar
)
