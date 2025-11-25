cmake_minimum_required(VERSION 3.23)

# To build the project, run the following commands in the terminal: 
# cmake --build . --target 'clean' cmake -S ../src -B . -DCMAKE_BUILD_TYPE=Release
# cmake --build . --parallel 6 --config Release

# flatc --cpp rz_write_flatbuffers.fbs -- output to includes/ flatc --json
# [--raw-binary] rz_write_flatbuffers.fbs -- rz_write_flatbuffers.bin 
# flatc --jsonschema rz_write_flatbuffers.fbs

project(
  rz_write_flatbuffers
  VERSION 0.1.0
  DESCRIPTION "Plugin to export Metadata to FlatBuffers"
  HOMEPAGE_URL "https://github.com/Zheng-Bote/rz_write_flatbuffer"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_NAME "${PROJECT_NAME}")
set(PROG_LONGNAME "write FlatBuffer")
set(PROG_AUTHOR "ZHENG Bote")
set(PROG_ORGANIZATION_NAME "ZHENG Robert")
set(PROG_ORGANIZATION_DOMAIN "net.hase-zheng")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_CXX_COMPILER clang++)
endif()

add_subdirectory(configure)

include(FetchContent)

FetchContent_Declare(
  FlatBuffers
  GIT_REPOSITORY "https://github.com/google/flatbuffers.git"
  GIT_TAG "v25.9.23"
  GIT_SHALLOW YES
  Overwrite_FIND_PACKAGE)

find_package(Flatbuffers REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

set(SCHEMA_FILE ${CMAKE_SOURCE_DIR}/schemas/rz_write_flatbuffers.fbs)
set(GENERATED_HEADER
    ${CMAKE_CURRENT_BINARY_DIR}/rz_write_flatbuffers_generated.h)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/includes)
set(OUTPUT_HEADER ${OUTPUT_DIR}/rz_write_flatbuffers_generated.h)

# 1. FlatBuffers generetion
add_custom_command(
  OUTPUT ${GENERATED_HEADER}
  COMMAND flatc --cpp -o ${CMAKE_CURRENT_BINARY_DIR} ${SCHEMA_FILE}
  DEPENDS ${SCHEMA_FILE}
  COMMENT "Generating FlatBuffers header...")

# 1. Header copy to src/includes
add_custom_command(
  OUTPUT ${OUTPUT_HEADER}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${GENERATED_HEADER} ${OUTPUT_HEADER}
  DEPENDS ${GENERATED_HEADER}
  COMMENT "Copying generated header to ${OUTPUT_HEADER}...")

# 1. create Target for both
add_custom_target(MySchemaHeaders DEPENDS ${OUTPUT_HEADER})

# add_executable(${PROJECT_NAME} main.cpp)
add_library(
  ${PROJECT_NAME} SHARED
  rz_write_flatbuffers.cpp includes/rz_write_flatbuffers.hpp
  includes/rz_config.hpp includes/rz_photo-gallery_plugins.hpp)

add_executable(test_write test_write.cpp includes/rz_config.hpp
                    includes/rz_photo-gallery_plugins.hpp
                    includes/rz_write_flatbuffers_generated.h)
target_compile_features(test_write PUBLIC cxx_std_23)
target_link_libraries(test_write PUBLIC flatbuffers)
target_link_libraries(test_write PRIVATE Qt6::Core)

add_executable(test_read test_read.cpp includes/rz_write_flatbuffers_generated.h)
target_compile_features(test_read PUBLIC cxx_std_23)
target_link_libraries(test_read PUBLIC flatbuffers)
target_link_libraries(test_read PRIVATE Qt6::Core)

add_dependencies(${PROJECT_NAME} MySchemaHeaders)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

target_link_libraries(${PROJECT_NAME} PUBLIC flatbuffers)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)

target_compile_definitions(${PROJECT_NAME} PRIVATE RZ_WRITE_FLATBUFFER_LIBRARY)
