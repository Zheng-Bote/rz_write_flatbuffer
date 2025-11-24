cmake_minimum_required(VERSION 3.23)

# To build the project, run the following commands in the terminal:
# cmake --build . --target 'clean'
# cmake -S ../src -B . -DCMAKE_BUILD_TYPE=Release
# cmake --build . --parallel 6 --config Release 

project(FlatBuffers 
VERSION 0.1.0 
LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_CXX_COMPILER clang++)
endif()

include (FetchContent)

FetchContent_Declare(
  FlatBuffers
  GIT_REPOSITORY "https://github.com/google/flatbuffers.git"
  GIT_TAG        "v25.9.23"
  GIT_SHALLOW    YES
  Overwrite_FIND_PACKAGE
)

find_package(Flatbuffers REQUIRED)

add_executable(${PROJECT_NAME} main.cpp includes/rz_write_flatbuffers_generated.h)


target_link_libraries(${PROJECT_NAME} PUBLIC flatbuffers)