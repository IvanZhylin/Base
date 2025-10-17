cmake_minimum_required(VERSION 3.10)
project(TicTacToe VERSION 1.0 DESCRIPTION "TicTacToe Game")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(TicTacToe ${SRCS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(TicTacToe PRIVATE DEBUG)
    target_compile_options(TicTacToe PRIVATE -g -O0 -Wall -Wextra)
else()
    target_compile_options(TicTacToe PRIVATE -O2)
endif()

# Настройка Doxygen 
find_package(Doxygen)
if(DOXYGEN_FOUND)
    # Создаем директорию для документации
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    
    # Настраиваем Doxygen
    set(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs)
    
    # Создаем кастомную цель для документации
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/docs/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()